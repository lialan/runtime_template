#include <iostream>
#include <sstream>
#include <fstream>
#include <fmt/core.h>

#include <dlfcn.h>

void gen_source_code(std::string t1, std::string t2, std::string file) {
  std::cout << "Generating library function source code:" << std::endl;

  std::string template_function =
"#include <iostream>\n"
"#include <typeinfo>\n\n"
"template <typename T1, typename T2>\n"
"void foo(T1 *a, T2 *b) { \n"
"std::cout<< typeid(a).name() << std::endl;\n"
"std::cout<< typeid(b).name() << std::endl;\n"
"}\n";

  std::stringstream ss;
  ss << template_function;

  // specialize template
  //ss << fmt::format("template<> void foo<{0}, {1}>({0} *a, {1} *b);", t1, t2) << std::endl;

  // wrapper function:
  ss << fmt::format("extern \"C\" void foowrapper({0} *a, {1} *b)", t1, t2) << std::endl;
  ss << "{" << std::endl;
  ss << fmt::format("    foo<{0}, {1}>(a, b);", t1, t2) << std::endl;
  ss << "}" << std::endl << std::endl;

  std::cout << ss.str() << std::endl;
  
  std::ofstream out(file);
  out << ss.str();
  out.close();
}

void gen_dylib(std::string infile, std::string outfile) {
  std::system(fmt::format("clang++-12 -shared -fPIC -o {0} {1}", outfile, infile).c_str());
}

void *load_dylib(std::string sofile) {
  std::cout << "trying to load library." << std::endl;
  void *handle = dlopen(sofile.c_str(), RTLD_NOW);
  if (!handle) {
    std::cerr << "Cannot open library: " << dlerror() << '\n';
    abort();
  }
  std::cout << "Successfully loaded library.\n" << std::endl;
  return handle;
}

void call_dylib(void* handle, std::string name) {
  dlerror();
  using wrapper_t = void (*)(void*, void*);
  // find symbol entry:
  wrapper_t wrapper_ptr = (wrapper_t)(dlsym(handle, "foowrapper"));
  const char* dlsym_error = dlerror();

  if (dlsym_error) {
    std::cout << "failed to load symbol (" << "foowrapper" << ")" << std::endl;
    dlclose(handle);
    abort();
  }

  int a = 10;
  float b = 20.0;

  wrapper_ptr((void*)(&a), (void*)(&b));
}

int main(int argc, char *argv[]) {
  std::string t1(argv[1]);
  std::string t2(argv[2]);

  std::string source_file = "/tmp/template.cc";
  std::string dylib_file = "/tmp/templatelib.so";

  gen_source_code(t1,t2, source_file);

  gen_dylib(source_file, dylib_file);

  void *dylib = load_dylib(dylib_file);

  call_dylib(dylib, "foowrapper");

  return 0;
}
