#ifndef VFP_RUNFILES_H
#define VFP_RUNFILES_H

#ifdef __cplusplus
extern "C" {
#endif

char *vfp_runfiles_resolve(const char *argv0, const char *workspace,
                            const char *relative_path);

#ifdef __cplusplus
}
#endif

#endif
