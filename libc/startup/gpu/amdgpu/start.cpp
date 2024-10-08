//===-- Implementation of crt for amdgpu ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "config/gpu/app.h"
#include "src/__support/GPU/utils.h"
#include "src/__support/RPC/rpc_client.h"
#include "src/__support/macros/config.h"
#include "src/stdlib/atexit.h"
#include "src/stdlib/exit.h"

extern "C" int main(int argc, char **argv, char **envp);

namespace LIBC_NAMESPACE_DECL {

// FIXME: Factor this out into common logic so we don't need to stub it here.
void teardown_main_tls() {}

DataEnvironment app;

extern "C" uintptr_t __init_array_start[];
extern "C" uintptr_t __init_array_end[];
extern "C" uintptr_t __fini_array_start[];
extern "C" uintptr_t __fini_array_end[];

using InitCallback = void(int, char **, char **);
using FiniCallback = void(void);

static void call_init_array_callbacks(int argc, char **argv, char **env) {
  size_t init_array_size = __init_array_end - __init_array_start;
  for (size_t i = 0; i < init_array_size; ++i)
    reinterpret_cast<InitCallback *>(__init_array_start[i])(argc, argv, env);
}

static void call_fini_array_callbacks() {
  size_t fini_array_size = __fini_array_end - __fini_array_start;
  for (size_t i = fini_array_size; i > 0; --i)
    reinterpret_cast<FiniCallback *>(__fini_array_start[i - 1])();
}

} // namespace LIBC_NAMESPACE_DECL

extern "C" [[gnu::visibility("protected"), clang::amdgpu_kernel,
             clang::amdgpu_flat_work_group_size(1, 1),
             clang::amdgpu_max_num_work_groups(1)]] void
_begin(int argc, char **argv, char **env) {
  __atomic_store_n(&LIBC_NAMESPACE::app.env_ptr,
                   reinterpret_cast<uintptr_t *>(env), __ATOMIC_RELAXED);
  // We want the fini array callbacks to be run after other atexit
  // callbacks are run. So, we register them before running the init
  // array callbacks as they can potentially register their own atexit
  // callbacks.
  LIBC_NAMESPACE::atexit(&LIBC_NAMESPACE::call_fini_array_callbacks);
  LIBC_NAMESPACE::call_init_array_callbacks(argc, argv, env);
}

extern "C" [[gnu::visibility("protected"), clang::amdgpu_kernel]] void
_start(int argc, char **argv, char **envp, int *ret) {
  // Invoke the 'main' function with every active thread that the user launched
  // the _start kernel with.
  __atomic_fetch_or(ret, main(argc, argv, envp), __ATOMIC_RELAXED);
}

extern "C" [[gnu::visibility("protected"), clang::amdgpu_kernel,
             clang::amdgpu_flat_work_group_size(1, 1),
             clang::amdgpu_max_num_work_groups(1)]] void
_end(int retval) {
  // Only a single thread should call `exit` here, the rest should gracefully
  // return from the kernel. This is so only one thread calls the destructors
  // registred with 'atexit' above.
  LIBC_NAMESPACE::exit(retval);
}
