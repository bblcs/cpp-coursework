#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace Emulator {
enum Reg { R0, R1, R2, R3 };

class EmulatorState;

// TODO: implement all instructions listed in ISA. This class should be base
// class for all insturctions
class Instruction {
public:
  virtual void eval(EmulatorState &emul) = 0;
  virtual ~Instruction() {};
};

struct EmulatorState {
  static const size_t regs_size = 4;
  static const size_t mem_size = 1024;

  std::vector<int> registers_;
  std::vector<int> mem_;

  size_t pc_ = 0; // program counter register

  EmulatorState() : registers_(regs_size, 0), mem_(mem_size, 0) {}
};

#define DEF_REGREG(name, EVAVA)                                                \
  class name##Reg : public Instruction {                                       \
    const size_t i_;                                                           \
    const size_t j_;                                                           \
                                                                               \
  public:                                                                      \
    name##Reg(size_t i, size_t j) : i_(i), j_(j) {}                            \
    void eval(EmulatorState &emul) {                                           \
      EVAVA;                                                                   \
      ++emul.pc_;                                                              \
    }                                                                          \
  };

#define DEF_REGIMM(name, EVAVA)                                                \
  class name##Imm : public Instruction {                                       \
    const size_t i_;                                                           \
    const int x_;                                                              \
                                                                               \
  public:                                                                      \
    name##Imm(size_t i, int x) : i_(i), x_(x) {}                               \
    void eval(EmulatorState &emul) {                                           \
      EVAVA;                                                                   \
      ++emul.pc_;                                                              \
    }                                                                          \
  };

#define DEF_EQ_X(name, X)                                                      \
  DEF_REGREG(name, emul.registers_[i_] X## = emul.registers_[j_])              \
  DEF_REGIMM(name, emul.registers_[i_] X## = x_);

DEF_EQ_X(Mov, );
DEF_EQ_X(Add, +);
DEF_EQ_X(Sub, -);
DEF_EQ_X(Mul, *);
DEF_EQ_X(Div, /);
DEF_REGIMM(Load, emul.registers_[i_] = emul.mem_[x_])
DEF_REGIMM(Store, emul.mem_[x_] = emul.registers_[i_])

class Jmp : public Instruction {
  const int x_;

public:
  Jmp(int x) : x_(x) {}
  void eval(EmulatorState &emul) { emul.pc_ = x_; }
};

class Jmpz : public Instruction {
  const int x_;

public:
  Jmpz(int x) : x_(x) {}
  void eval(EmulatorState &emul) {
    if (emul.registers_[0] == 0) {
      emul.pc_ = x_;
    } else {
      ++emul.pc_;
    }
  }
};

#define EQX_IF(name)                                                           \
  if (tok == #name) {                                                          \
    ss >> a;                                                                   \
    ss >> b;                                                                   \
                                                                               \
    if (b[0] == 'R') {                                                         \
      v.push_back(std::make_unique<name##Reg>(std::stoi(a.substr(1)),          \
                                              std::stoi(b.substr(1))));        \
    } else {                                                                   \
      v.push_back(                                                             \
          std::make_unique<name##Imm>(std::stoi(a.substr(1)), std::stoi(b)));  \
    }                                                                          \
  }

#define JMP_IF(name)                                                           \
  if (tok == #name) {                                                          \
    ss >> a;                                                                   \
                                                                               \
    v.push_back(std::make_unique<name>(std::stoi(a)));                         \
  }
#define SL_IF(name, class)                                                     \
  if (tok == #name) {                                                          \
    ss >> a;                                                                   \
    ss >> b;                                                                   \
                                                                               \
    v.push_back(                                                               \
        std::make_unique<class>(std::stoi(a.substr(1)), std::stoi(b)));        \
  }

/* This function accepts the program written in the vrisc assembly
 * If the input program is correct, returns sequence of insturction,
 * corresponding to the input program. If the input text is incorrect, the
 * behaviour is undefined
 */
std::vector<std::unique_ptr<Instruction>> parse(const std::string &program) {
  std::istringstream ss(program);
  std::vector<std::unique_ptr<Instruction>> v;
  std::string tok;

  while (ss >> tok) {
    std::string a;
    std::string b;

    EQX_IF(Mov)
    EQX_IF(Add)
    EQX_IF(Sub)
    EQX_IF(Mul)
    EQX_IF(Div)
    SL_IF(Load, LoadImm)
    SL_IF(Store, StoreImm)
    JMP_IF(Jmp)
    JMP_IF(Jmpz)
  }

  return v;
}

/* Emulate receive a program, written in the vrisc assembly,
 * in case of the correct program, emulate returns R0 value at the end of the
 * emulation. If the program is incorrect, that is, either its text is not vrisc
 * assembly language or it contains UB(endless cycles), the behaviour of emulate
 * if also undefined. Handle these cases in any way.
 */
int emulate(const std::string &program_text) {
  // Feel free to change code of this function
  std::vector<std::unique_ptr<Instruction>> program = parse(program_text);

  EmulatorState state;

  while (state.pc_ < program.size()) {
    program[state.pc_]->eval(state);
  }

  return state.registers_[R0];
}
} // namespace Emulator

int main() {
  // For writing test you can write programs directly in raw string literals
  std::string factorial = R"(
    Mov R0 5
    Mov R1 1
    Jmpz 6

    Mul R1 R0
    Sub R0 1
    Jmp 2

    Mov R0 R1
  )";

  // The result should be 120
  assert(Emulator::emulate(factorial) == 120);
  assert(Emulator::emulate(factorial) == 120);
  assert(Emulator::emulate(factorial) == 120);

  std::string tuff = R"(
    Mov R1 19
    Store R1 12
    Load R0 12
  )";

  assert(Emulator::emulate(tuff) == 19);
  assert(Emulator::emulate(tuff) == 19);
  assert(Emulator::emulate(factorial) == 120);
  assert(Emulator::emulate(tuff) == 19);
  assert(Emulator::emulate(tuff) == 19);
  assert(Emulator::emulate(tuff) == 19);
  assert(Emulator::emulate(factorial) == 120);
  assert(Emulator::emulate(tuff) == 19);
  assert(Emulator::emulate(tuff) == 19);
  assert(Emulator::emulate(tuff) == 19);
  assert(Emulator::emulate(factorial) == 120);
  assert(Emulator::emulate(tuff) == 19);

  return 0;
}
