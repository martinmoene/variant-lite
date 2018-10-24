from conans import ConanFile

class VariantLiteConan(ConanFile):
    version = "1.1.0"
    name = "variant-lite"
    description = "A single-file header-only version of a C++17-like variant, a type-safe union for C++98, C++11 and later"
    license = "Boost Software License - Version 1.0. http://www.boost.org/LICENSE_1_0.txt"
    url = "https://github.com/martinmoene/variant-lite.git"
    exports_sources = "include/nonstd/*", "LICENSE.txt"
    build_policy = "missing"
    author = "Martin Moene"

    def package_info(self):
        self.info.header_only()
