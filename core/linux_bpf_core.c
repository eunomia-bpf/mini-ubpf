#include "type-fixes.h"
#include "linux-errno.h"
#include "linux_bpf.h"
#include "bpf_jit_arch.h"
#include "ebpf_vm.h"
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#ifdef __linux__
// for mmap and munmap, to let jit code be executable
#include <sys/mman.h>
#include <time.h>
#endif

/* Base function for offset calculation. Needs to go into .text section,
 * therefore keeping it non-static as well; will also be used by JITs
 * anyway later on, so do not let the compiler omit it. This also needs
 * to go into kallsyms for correlation from e.g. bpftool, so naming
 * must not change.
 */
__attribute__((__noinline__)) u64 __bpf_call_base(u64 r1, u64 r2, u64 r3, u64 r4, u64 r5)
{
	return 0;
}

/* The logic is similar to BPF_PROG_RUN, but with an explicit
 * rcu_read_lock() and migrate_disable() which are required
 * for the trampoline. The macro is split into
 * call _bpf_prog_enter
 * call prog->bpf_func
 * call __bpf_prog_exit
 */
u64 __bpf_prog_enter(void)
{
#ifdef __linux__
	u64 start = clock();
#elif
	u64 start = 0;
#endif
	printf("__bpf_prog_enter %" PRIu64 "\n", start);
	return start;
}

void __bpf_prog_exit(struct ebpf_vm *prog, u64 start)
{
	// do nothing
	printf("__bpf_prog_exit %" PRIu64 "u\n", start);
}

int bpf_jit_get_func_addr(const struct ebpf_vm *prog,
			  const struct bpf_insn *insn, bool extra_pass,
			  u64 *func_addr, bool *func_addr_fixed)
{
	s32 imm = insn->imm;
	u8 *addr;

	*func_addr_fixed = insn->src_reg != BPF_PSEUDO_CALL;
	if (!*func_addr_fixed) {
		/* Place-holder address till the last pass has collected
		 * all addresses for JITed subprograms in which case we
		 * can pick them up from prog->aux.
		 */
		if (!extra_pass)
			addr = NULL;
		else
			return -EINVAL;
	} else {
		/* Address of a BPF helper call. Since part of the core
		 * kernel, it's always at a fixed location. __bpf_call_base
		 * and the helper with imm relative to it are both in core
		 * kernel.
		 */
		addr = (u8 *)__bpf_call_base + imm;
	}

	*func_addr = (unsigned long)addr;
	return 0;
}
