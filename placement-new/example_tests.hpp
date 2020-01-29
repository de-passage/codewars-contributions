namespace pn = placing_new_objects;

// In these tests, record<size_t S, size_t A = S> is a data structure of 
// size S with alignment A that records its constructor and destructor calls.

Describe(construct)
{
  void SetUp() override {
    perform_black_magic(); // initialization details you don't need to worry about
  }

  It(should_call_constructors_as_many_times_as_possible) {

    char array[12] = {0};
    pn::construct<record<1>>(array); 
    Assert::That(ctor_calls.size(), Equals(12));

    for (int i = 0; i < 12; ++i)
      Assert::That(ctor_calls[i], Equals(array + i));
  }

  It(should_write_on_the_buffer) {

    char array[12] = {0};
    pn::construct<char>(array, '!'); 
    for (int i = 0; i < 12; ++i)
      Assert::That(array[i], Equals('!'));
  }

  It(should_deal_with_buffers_too_small) {

    std::aligned_storage_t<8, 8> buffer;

    pn::construct<record<16, 8>>(buffer);

    Assert::That(ctor_calls.size(), Equals(0));
  }

  It(should_ignore_excedentary_space) {

    char array[9] = { '!' };

    pn::construct<record<2>>(array);

    Assert::That(ctor_calls.size(), Equals(4));

    for (int i = 0; i < 4; ++i)
      Assert::That(ctor_calls[i], Equals(array + i * 2));
  }
};

Describe(destruct) {

  void SetUp() override {
    perform_black_magic();
  }

  It(should_call_destructors_as_many_times_as_possible) {

    record<1> array[15];

    pn::destruct<record<1>>(array);

    Assert::That(dtor_calls.size(), Equals(15));
  }

  It(should_deal_with_buffers_too_small) {

    std::aligned_storage_t<64, 64> buffer;

    pn::destruct<record<128, 64>>(buffer);

    Assert::That(dtor_calls.size(), Equals(0));
  }

  It(should_ignore_excedentary_space) {

    char array[11] = { '!' };

    pn::destruct<record<2>>(array);

    Assert::That(dtor_calls.size(), Equals(5));

    for (int i = 0; i < 5; ++i)
      Assert::That(dtor_calls[i], Equals(array + i * 2));
  }
};
