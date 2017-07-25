#pragma once
#include <cstddef>
#include <mutex>

#define MIN(A,B) ((A)<(B)?(A):(B))

class RingBuffer {
public:
  explicit RingBuffer(size_t size);
  ~RingBuffer();

  inline size_t dataLength() const {
    /*
       _____out______in_____
      |_空闲_|__占用__|_空闲_|   occupied = in - out

       _____in______out_____
      |_占用_|__空闲__|_占用_|   occupied = in + size - out
      由于是无符号类型, in < out 时会自动 wrap-around
    */
    return in_ - out_;
  }

  /*
     _____out______in_____
    |_空闲_|__占用__|_空闲_|   free_size = out + size - in

     _____in______out_____
    |_占用_|__空闲__|_占用_|   free_size = out - in
  */
  void putData(unsigned char* data, size_t datalen);

  void getData(unsigned char* buf, size_t datalen);

private:
  unsigned char *buffer_;
  size_t size_; // 缓冲区大小
  size_t in_;
  size_t out_;
  std::mutex mu_;

  inline bool is_power_of_2(size_t x) {
    return x != 0 && (x & (x-1)) == 0;
  }
}