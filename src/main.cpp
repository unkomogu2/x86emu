#include <Emulator.hpp>
#include <fstream>
#include <iostream>

static const size_t MEMORY_SIZE = 1024 * 1024;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "usage: px86 filename" << std::endl;
    return 1;
  }
  Emulator emu(MEMORY_SIZE, 0x0000, 0x7c00);
  std::ifstream file(argv[1], std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "[error] failed to open file" << std::endl;
    return 1;
  }

  file.read(emu.memory, sizeof(uint8_t) * 1024);

  emu.init_instructions();
  while (emu.eip < MEMORY_SIZE) {
    uint8_t code = emu.get_code8(0);
    printf("EIP = %X, Code = %02X\n", emu.eip, code);

    emu.instructions[code]();

    if (emu.eip == 0x00) {
      printf("\n\nend of program.\n\n");
      break;
    }
  }

  emu.dump_registers();
}
