/**
 * @file      riscv_isa_helper.H
 *             
 *
 * @version   1.0
 * @date      August 2016
 *
 *
 * @brief     This is a helper header file included directly
 *            in the generated riscv_isa.H file. Here we put
 *            data structures and functions used throughout
 *            the riscv_isa.cpp (riscv instruction behaviors).
 **/

typedef union {
  float f;
  struct {
    unsigned int mantisa : 23;
    unsigned int exponent : 8;
    unsigned int sign : 1;
  } parts;
  uint32_t data;
} float_cast;

typedef union {
  double d;
  struct {
    unsigned long long mantisa : 52;
    unsigned long long exponent : 11;
    unsigned int sign : 1;
  } parts;
  uint64_t data;
} double_cast;


 static inline int sign_extend(int word, int word_length){
  int sign_ext;
  int m = 0xFFFFFFFF;
  if((word>>(word_length-1) == 1))
    sign_ext = word | (m<<word_length);
  else 
    sign_ext = word;
  return sign_ext;
 }

 inline double load_double(uint32_t index) {
  double res;
  uint64_t input = ((uint64_t)RBF[index*2]) + (((uint64_t)RBF[index*2 + 1]) << 32);
  memcpy(&res, &input, sizeof(input));
  return res;
}

inline void save_double(double input, uint32_t index) {
  uint64_t temp;
  memcpy(&temp, &input, sizeof(temp));
  RBF[index*2] = temp & 0xFFFFFFFF;
  RBF[index*2 + 1] = temp >> 32;
}

inline float load_float(uint32_t index) {
  float res;
  memcpy(&res, &RBF[index], sizeof(uint32_t));
  return res;
}

inline void save_float(float input, uint32_t index) {
  memcpy(&RBF[index], &input, sizeof(uint32_t));
}

static bool custom_isnan(double var) {
  return var != var;
}

#define csr_map(i) i==0x1? fflags:\
                      i==0x2? frm:\
                        i==0x3? fcsr:\
                          fflags;
