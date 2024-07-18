// gcc test1.c -lseccomp
#include <stdio.h>
#include <seccomp.h>
int main() {
    scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_KILL);
    if (ctx < 0)
        perror("seccomp_init");

    if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 0))
        perror("seccomp_rule_add");
    if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(read), 0))
        perror("seccomp_rule_add");
    if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 0))
        perror("seccomp_rule_add");
    if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(exit), 0))
        perror("seccomp_rule_add");
    if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(exit_group), 0))
        perror("seccomp_rule_add");

    if (seccomp_load(ctx))
        perror("seccomp_load");

    seccomp_release(ctx);

    return 0;
}
