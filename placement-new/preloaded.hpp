#include <csignal>
#include <memory>

std::vector<void*> dtor_calls;
std::vector<void*> ctor_calls;


struct record_base {
  protected:
  record_base() {
    ctor_calls.push_back(this);
  }

  ~record_base() {
    dtor_calls.push_back(this);
  }
};

template<std::size_t Size, std::size_t Alignment = Size>
struct alignas(Size) record : record_base {
  char _padding[Size];
};

void slay_nose_demons(int s) {
  switch(s) {
    case SIGSEGV:
        fprintf(stderr, "Oops! Your program segfaulted! This means that you have been reading "
                  "or writing to a memory area you shouldn't be accessing and this is "
                  "BAD!\nYou triggered undefined behavior, and demons started flying from "
                  "your nose! Fortunately I slayed them for you, but since your code has "
                  "been misbehaving I have to shut it down too... And since I was called after "
                  "the facts I can't even tell you what happened in the first place!\n\n"
                  "I'm sure you'll find what's wrong though! C++ is a tricky language but "
                  "there's always a rational explanation to what's happening.\n"
                  "Try adding logs or following the execution step by step in a debugger, and look out for "
                  "sketchy pointer manipulation! Other causes may be double deletion (calling delete on an "
                  "already freed pointer), or messy ownership of memory between two different objects/threads "
                  "(why are you using threads?).\n"
                  "Also keep in mind that the root cause of a memory corruption may be located a long time before"
                  " your program crashes. Try breaking out every non-trivial computation and write unit tests to "
                  "double-check that your code is actually doing what you think it is.\n\n"
                  "All in all raw memory manipulation is a very, VERY tricky thing and mistakes happen all the time."
                  " I personally messed up twice while writing the solution and the tests because I misread the "
                  "documentation!\n\nDon't despair, and good luck!\n");
        std::exit(EXIT_FAILURE);
  }
}

void perform_black_magic() {
  static bool defined = false;
  if (!defined) {
    defined = true;
    std::signal(SIGSEGV, slay_nose_demons);
  }
    ctor_calls.clear();
    dtor_calls.clear();
}
