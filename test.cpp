#include <iostream>
#include <sstream>
#include <fmt/core.h>

void gen_source_code(std::string t1, std::string t2, std::string file) {

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
  ss << "}" << std::endl;

  std::cout << ss.str() << std::endl;
  
  std::ofstream out(file);
  out << ss.str();
  out.close();
}

void gen_dylib(std::string infile, std::string outfile) {
  std::system(fmt::format("clang++12 -shared -fPIC -o {0} {1}", infile, outfile));
}

int main(int argc, char *argv[]) {
  std::string t1(argv[1]);
  std::string t2(argv[2]);

  std::string source_file = "/tmp/template.cc";
  std::string dylib_file = "/tmp/templatelib.so";

  gen_source_code(t1,t2, source_file);

  gen_dylib(source_file, outfile);

  load_dylib(outfile);

  call_dylib("int", "float");

  return 0;
}
