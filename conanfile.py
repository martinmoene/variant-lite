from conans import ConanFile, CMake

class VariantLiteConan(ConanFile):
    version = "1.2.0"
    name = "variant-lite"
    description = "A single-file header-only version of a C++17-like variant, a type-safe union for C++98, C++11 and later"
    license = "Boost Software License - Version 1.0. http://www.boost.org/LICENSE_1_0.txt"
    url = "https://github.com/martinmoene/variant-lite.git"
    exports_sources = "include/nonstd/*", "CMakeLists.txt", "cmake/*", "LICENSE.txt"
    build_policy = "missing"
    author = "Martin Moene"

    def build(self):
        """Avoid warning on build step"""
        pass

    def package(self):
        """Run CMake install"""
        cmake = CMake(self)
        cmake.definitions["VARIANT_LITE_OPT_BUILD_TESTS"] = "OFF"
        cmake.definitions["VARIANT_LITE_OPT_BUILD_EXAMPLES"] = "OFF"
        cmake.configure()
        cmake.install()

    def package_info(self):
        self.info.header_only()
