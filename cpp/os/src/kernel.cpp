void
printf(char* str)
{
  static unsigned short* video_memory = (unsigned short*)0xb8000;

  for (int i = 0; str[i] != '\0'; ++i) {
    video_memory[i] = (video_memory[i] & 0xFF00) | str[i];
  }
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void
call_ctors()
{
  for (constructor* i = &start_ctors; i != &end_ctors; i++) {
    (*i)();
  }
}

extern "C" void
kernel_main(void* multiboot_struct, unsigned int magicnumber)
{
  printf("Hello, World from kernel.cpp file!");
  while (1)
    ;
}
