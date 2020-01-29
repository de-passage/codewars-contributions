In this kata we'll explore a lesser-known feature of C++ : building objects at specific memory locations. 

---

# First Task : construct

Implement a template function called `construct` which takes a buffer and some parameters and construct inside of the buffer as many well-aligned instances of a given type as possible.

`construct` takes several template parameters : the type `Type` of the objects to build, the type `Buffer` of some contiguous range of memory, and a parameter pack.
The first parameter of `construct` will be a reference to the object _on_ which you'll build the desired `Type` objects, the rest are the parameters you need to give to the constructor of `Type`.

Warning ! You may be required to leave some space inside of `buffer` unused! Pay also attention to the alignment of `Buffer` with respect to `Type`!

`Type` will never be an array or pointer type, no need to worry about that. It may be non trivial and have constructors/destructors with side effects though!

# Second Task : destruct

Implement a template function called `destruct` taking a buffer filled with as many and empties it of its content. Memory doesn't need to be rewritten explicitely.
`destruct` takes two template parameters : `Type`, the type of the objects contained in the buffer, and `Buffer`, the type of the buffer itself.

The buffers given to `destruct` can be considered as having been through `construct` beforehand. The same rules apply.

---

# Trivia 

The techniques used to solve this kata are somewhat obscure and rarely useful, so do not abuse them ! They are however an indispensable part of C++, making possible things like std::optional (which doesn't make any dynamic memory allocation) or std::vector with non-copyable/non-default-constructible types. It is also the basis for the need for allocators, which are omnipresent in the standard library.

# Hints

### First hint
Most people understand what type size is, but you may need to read up on [alignment](https://en.cppreference.com/w/cpp/language/object#Alignment)!

### Second hint
You are probably aware of the `new` operator, which does two things : allocating a block of memory fitting the type of the object that you are trying to construct and then calls the constructor of the given type. 
What many people are not aware of is that `new` is not a magical language construct. It is an operator with several overloads !

For example `new` normally throws an exception of type `std::bad_alloc` if memory allocation failed, but there is also a version of `new` returning `nullptr` on failure!
```cpp
MyClass* my_object = new(std::nothrow) MyClass;

if (my_object == nullptr) {
  std::terminate();
}
```

To solve this kata, you'll need one of the many versions of `new`. I leave it to you to find it and understand how to use it. For all things C++, [cppreference](https://en.cppreference.com/w/cpp/memory/new/operator_new) is a good place to start.

