#include "vfp_runfiles.h"

#include <memory>
#include <string>
#include <string.h>

#include "tools/cpp/runfiles/runfiles.h"

char *vfp_runfiles_resolve(const char *argv0, const char *workspace,
                            const char *relative_path) {
    if (!workspace || !relative_path) {
        return nullptr;
    }

    std::unique_ptr<bazel::tools::cpp::runfiles::Runfiles> runfiles(
        bazel::tools::cpp::runfiles::Runfiles::Create(argv0 ? argv0 : "",
                                                      nullptr));
    if (!runfiles) {
        return nullptr;
    }

    std::string key = std::string(workspace) + "/" + relative_path;
    std::string resolved = runfiles->Rlocation(key);
    if (resolved.empty()) {
        return nullptr;
    }

    char *out = static_cast<char *>(malloc(resolved.size() + 1));
    if (!out) {
        return nullptr;
    }

    memcpy(out, resolved.c_str(), resolved.size() + 1);
    return out;
}
