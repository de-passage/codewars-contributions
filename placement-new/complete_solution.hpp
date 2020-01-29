#import <memory>

namespace placing_new_objects {
  /** \brief Constructs as many well aligned objects as possible of type 'Type' inside of 'buffer'
      
      \param[in, out] buffer The buffer on which to construct the objects
      \param[in] parameters The parameters to give to the constructor of 'Type'
  */ 
  template<typename Type, typename Buffer, typename... Parameters>
  void construct(Buffer& buffer, const Parameters&... parameters) {
  
    // We first take the address of the buffer. Even though I didn't put this trap in the tests, 
    // technically one could overload operator& on Buffer to return something else.
    // std::adressof ensures that we actually get the value we want.
    void* start = std::addressof(buffer);
    
    // The available space in the buffer is simply the size of the buffer itself
    std::size_t available_space = sizeof(Buffer);
    
    // std::align will find the first address aligned to the first parameter that can 
    // fit the second parameter worth of memory, starting at the third parameter, within
    // the fourth parameter space constraint. If there is no such address, the function 
    // will return nullptr and the loop will stop
    while(std::align(alignof(Type), sizeof(Type), start, available_space)) {
    
      // Placement new! We call the constructor of Type at the address we have. Inside of it, this == start
      ::new(start) Type(parameters...);
      
      // We then adjust the pointer and the available space to reflect the fact that we 'used'
      // the spot of the object we just constructed. We need to cast the pointer because 
      // arithmetic on void* is forbidden
      start = reinterpret_cast<char*>(start) + sizeof(Type);
      available_space -= sizeof(Type);
    }
  }

  /** \brief Destructs as many elements of type 'Type' inside of 'buffer' as there can be
      
      The elements are supposed to be well aligned and to fill as much of buffer as possible without overflowing
      
      \param[in, out] buffer The buffer containing the elements to destroy
  */
  template<typename Type, typename Buffer>
  void destruct(Buffer& buffer) {
    void* start = static_cast<void*>(std::addressof(buffer));
    std::size_t available_space = sizeof(Buffer);
    
    while(std::align(alignof(Type), sizeof(Type), start, available_space)) {
      
      // This function is identical to the previous one, with the exception of this line.
      // Instead of calling the constructor we call this time the destructor.
      reinterpret_cast<Type*>(start)->~Type();
      
      start = reinterpret_cast<char*>(start) + sizeof(Type);
      available_space -= sizeof(Type);
    }
  }
}