namespace pn = placing_new_objects;

Describe(construct)
{
  void SetUp() override {

    perform_black_magic();
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

  It(should_deal_with_misaligned_buffers) {
    std::aligned_storage_t<16, 8> storage;

    // The things we do for love...
    char* ptr = reinterpret_cast<char*>(&storage) + 2; // arbitrary offset to misalign the buffer voluntarily
    char (&buffer)[14] = *reinterpret_cast<char(*)[14]>(ptr);  // black magic

    pn::construct<record<8>>(buffer);

    Assert::That(ctor_calls.size(), Equals(1));
    Assert::That(ctor_calls[0], Equals(ptr + 6)); // 8 - 2 = 6 ! 
  }

  It(should_work_with_non_trivial_types) {
    using string = std::string;

    const char* const str = "should work with std::string";
    std::aligned_storage_t<sizeof(std::string), alignof(std::string)> buffer;

    pn::construct<std::string>(buffer, str);

    std::string& sref = *reinterpret_cast<std::string*>(&buffer);
    Assert::That(sref, Equals(str));
    sref.~string(); // I'm well mannered
  }

  struct some_multiparameter_constructor {
    some_multiparameter_constructor(int a, const std::string& s, bool b) 
      : i(a * s.size() - (b ? 3 : 5)) {}
      
    int i;
  };

  It(should_forward_all_constructor_arguments) {
    int i = 0;
    
    pn::construct<some_multiparameter_constructor>(i, 5, "Hello", true);
    Assert::That(i, Equals(22));
    
    pn::construct<some_multiparameter_constructor>(i, 2, "World", false);
    Assert::That(i, Equals(5));
    
    pn::construct<some_multiparameter_constructor>(i, 3, "!", true);
    Assert::That(i, Equals(0));
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

  It(should_deal_with_misaligned_buffers) {

    std::aligned_storage_t<16, 8> storage;

    char* ptr = reinterpret_cast<char*>(&storage) + 2; // arbitrary offset to misalign the buffer voluntarily
    char (&buffer)[14] = *reinterpret_cast<char(*)[14]>(ptr);  // black magic

    pn::destruct<record<8>>(buffer);

    Assert::That(dtor_calls.size(), Equals(1));
    Assert::That(dtor_calls[0], Equals(reinterpret_cast<char*>(&storage) + 8));
  }

  It(should_ignore_excedentary_space) {

    char array[11] = { '!' };

    pn::destruct<record<2>>(array);

    Assert::That(dtor_calls.size(), Equals(5));

    for (int i = 0; i < 5; ++i)
      Assert::That(dtor_calls[i], Equals(array + i * 2));
  }
};
