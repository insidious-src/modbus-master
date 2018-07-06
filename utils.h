#ifndef UTILS_H
#define UTILS_H

//******************************************************************//
namespace Utils {

//******************************************************************//
typedef enum _32bitDataByteOrder_t {
    noSwap = 0,                                                                         //No swap
    byteSwap,                                                                           //Byte swap
    wordSwap,                                                                           //Word swap
    byteWordSwap                                                                        //Byte and word swap
} _32bitDataByteOrder_t;                                                                //Byte order for 32-bit data
//******************************************************************//

//******************************************************************//
template <typename T>
T Int16RegTo32bitData(const uint16_t *msReg, const uint16_t *lsReg, _32bitDataByteOrder_t byteOrder) {
    T val = 0;
    const int swapLUT[4][4] = {
        {0, 1, 2, 3},															/*No swap*/
        {1, 0, 3, 2},															/*Byte swap*/
        {2, 3, 0, 1},															/*Word swap*/
        {3, 2, 1, 0}															/*Byte and word swap*/
    };
    const int *swapP = &swapLUT[byteOrder][0];                                  /*Swap LUT pointer*/
    uint8_t byteArray[4];
    byteArray[0] = *((uint8_t *)msReg + 1);
    byteArray[1] = *((uint8_t *)msReg + 0);
    byteArray[2] = *((uint8_t *)lsReg + 1);
    byteArray[3] = *((uint8_t *)lsReg + 0);
    *((uint8_t *)&val + 3) = byteArray[*(swapP + 0)];
    *((uint8_t *)&val + 2) = byteArray[*(swapP + 1)];
    *((uint8_t *)&val + 1) = byteArray[*(swapP + 2)];
    *((uint8_t *)&val + 0) = byteArray[*(swapP + 3)];
    return (val);
}
//******************************************************************//

}
//******************************************************************//

#endif // UTILS_H
