#include "vec_buffer.h"

#include <iostream>




int main() {

    std::vector<int> vec{1,2,3,4,5,6,7,8,9,10};

    VecBuffer<int> buf(10);

    buf.insertArray(vec);
    buf.insertData(86);
    buf.insertData(13);
    buf.insertData(666);




   std::cout << "Get Frame = " << buf.getFrame(3) << std::endl;
   auto range = buf.getFrameRange(0, 7);
   std::cout << "Range is  :  ";



   for(const auto el_range : range) {
    std::cout << el_range << " ";
   }
   std::cout << std::endl;


return 0;
}
