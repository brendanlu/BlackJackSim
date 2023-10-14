    
    #include <random>
    
    /* uniform integer sampling transform */
    
std::uniform_int_distribution<unsigned int> unifInt(0, width);

/* ** Fisher Yates algorithm */ 
unsigned int j; 
for (unsigned int i = width; i >= 1; --i) {
    unifInt.param(unifIntBounds_t(0, i));
    j = unifInt(rng);
    std::swap(*(begin+i), *(begin+j));
}