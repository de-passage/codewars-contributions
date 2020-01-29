namespace placing_new_objects {
  /** \brief Constructs as many well aligned objects as possible of type 'Type' inside of 'buffer'

      \param[in, out] buffer The buffer on which to construct the objects
      \param[in] parameters The parameters to give to the constructor of 'Type'
  */ 
  template<typename Type, typename Buffer, typename... Parameters>
  void construct(Buffer& buffer, const Parameters&... parameters) {
    /* Implement me! */
  }

  /** \brief Destructs as many elements of type 'Type' inside of 'buffer' as there can be

      The elements are supposed to be well aligned and to fill as much of buffer as possible without overflowing

      \param[in, out] buffer The buffer containing the elements to destroy
  */
  template<typename Type, typename Buffer>
  void destruct(Buffer& buffer) {
    /* Implement me! */
  }
}