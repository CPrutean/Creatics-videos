#include <cinttypes>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#define SMALLSSO 16

namespace SSO {

class string {
private:
  struct shortStr {
    char stackBuff[SMALLSSO];
  };

  struct longStr {
    char *ptr;
    size_t capacity;
  };

  size_t length;

  union {
    shortStr mShort;
    longStr mLong;
  };

public:
  friend std::ostream &operator<<(std::ostream &ref, string &str);
  string(const char *str) {
    length = strlen(str);
    if (length <= SMALLSSO) {
      std::copy(str, str + length, mShort.stackBuff);
    } else {
      mLong.ptr = new char[length];
      std::copy(str, str + length, mLong.ptr);
      mLong.capacity = length;
    }
  }

  size_t Length() { return length; }

  string(const string &other) {
    length = other.length;
    std::copy(other.mShort.stackBuff, other.mShort.stackBuff + SMALLSSO,
              mShort.stackBuff);
  }

  ~string() {}

  string(const string &&other) {
    length = other.length;
    std::copy(other.mShort.stackBuff, other.mShort.stackBuff + SMALLSSO,
              mShort.stackBuff);
  }

  void operator+=(const char *str) {
    size_t cStrLen = strlen(str);
    size_t newTotalLen = length + cStrLen;
    if (length < SMALLSSO) {
      if (newTotalLen > SMALLSSO) {
        char *temp = new char[newTotalLen + 1];
        std::copy(mShort.stackBuff, mShort.stackBuff + length, temp);
        std::copy(str, str + cStrLen, temp + length);
        temp[newTotalLen] = '\0';
        mLong.ptr = temp;
        mLong.capacity = newTotalLen;

      } else {
        std::copy(str, str + length, mShort.stackBuff + length);
        mShort.stackBuff[newTotalLen] = '\0';
      }
    } else {
      if (newTotalLen > mLong.capacity) {
        char *temp = new char[mLong.capacity * 2];
        size_t newCapacity = mLong.capacity * 2;

        std::copy(mLong.ptr, mLong.ptr + length, temp);
        std::copy(str, str + cStrLen, temp + length);
        mLong.capacity = newCapacity;
        delete[] mLong.ptr;
        mLong.ptr = temp;

      } else {
        std::copy(str, str + cStrLen, mLong.ptr + length);
      }
    }

    length = newTotalLen;
  }

  void operator+=(const string &str) {
    size_t newCapacity = length + sizeof(str);

    const char *strPtr;
    if (str.length > SMALLSSO) {
      strPtr = str.mLong.ptr;
    } else {
      strPtr = str.mShort.stackBuff;
    }
    size_t newTotalLen = length + str.length;
    size_t strLen = str.length;
    if (length < SMALLSSO) {
      if (newTotalLen >= SMALLSSO) {
        char *temp = new char[newTotalLen + 1];
        std::copy(mShort.stackBuff, mShort.stackBuff + length, temp);
        std::copy(strPtr, strPtr + strLen, temp + length);
        temp[newTotalLen] = '\0';
        mLong.ptr = temp;
        mLong.capacity = newTotalLen;
      } else {
        std::copy(strPtr, strPtr + strLen, mShort.stackBuff + length);
        mShort.stackBuff[newTotalLen] = '\0';
      }
    } else {
      if (newTotalLen >= mLong.capacity) {
        char *temp = new char[mLong.capacity * 2];
        size_t newCapacity = mLong.capacity * 2;
        std::copy(mLong.ptr, mLong.ptr + length, temp);
        std::copy(strPtr, strPtr + strLen, temp + length);
        temp[newTotalLen] = '\0';
        delete[] mLong.ptr;
        mLong.ptr = temp;
        mLong.capacity = newCapacity;
      } else {
        std::copy(strPtr, strPtr + strLen, mLong.ptr + length);
      }
    }
  }
};

std::ostream &operator<<(std::ostream &ref, SSO::string &str) {
  if (str.length <= SMALLSSO) {
    ref << str.mShort.stackBuff;
  } else {
    ref << str.mLong.ptr;
  }
  return ref;
}

} // namespace SSO

int main() {
  SSO::string str{"Testing"};
  SSO::string str2("1234578956437829-57823907845902");
  str += "123548390532490";
  str += "123";
  str += str2;

  std::cout << str << std::endl;
}
