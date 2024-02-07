#include "vec_buffer.h"

#include <iostream>



int main() {

    std::vector<int> vec{1,2,3,4,5};

    VecBuffer<int> buf(5);

    buf.insertArray(vec);
    buf.insertData(13);
    buf.insertData(86);
    buf.insertData(666);




   std::cout << "Get Frame = " << buf.getFrame(3) << std::endl;
   auto range = buf.getFrameRange(0, 4);
   std::cout << "Range is  :  ";



   for(int i = 0; i <= 4; ++i) {
    std::cout << range[i] << " ";
   }
   std::cout << std::endl;


return 0;
}
