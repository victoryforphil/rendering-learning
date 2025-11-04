cc_binary(
  name = "hello_triangle",
  srcs = ["hello_triangle.cc"],
  deps = [
    "@glfw//:glfw",
    "@glm//:glm",
    "//glad:glad",
  ],
  copts = ["-DGLM_ENABLE_EXPERIMENTAL"],
)

cc_binary(
  name = "hello_log",
  srcs = ["hello_log.cc"],
  deps = [
    "@spdlog//:spdlog",
  ],
)
