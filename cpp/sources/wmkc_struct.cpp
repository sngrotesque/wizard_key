#include <wmkc_struct.hpp>

static const wmkcByte byteOrder[] = {
    '@', // 按原样顺序
    '>', // 按大端序
    '<', // 按小端序
    '!'  // 网络端序（大端序）
};

static const wmkcByte formatSymbol[] = {
    'B', // unsigned char  -> uint8_t
    'H', // unsigned short -> uint16_t
    'I', // unsigned int   -> uint32_t
    'Q', // unsigned long  -> uint64_t
    'N', // unsigned long long -> size_t
    'f', // float
    'd'  // double
};

wmkcVoid wmkcStruct::verifySymbol(const std::string format, const wmkcSize args_length)
{
    const wmkcByte *bo_bp = byteOrder;
    const wmkcByte *bo_ep = byteOrder + sizeof(byteOrder);
    const wmkcByte *fy_bp = formatSymbol;
    const wmkcByte *fy_ep = formatSymbol + sizeof(formatSymbol);
    wmkcSize expected_length;
    wmkcBool bo_ok, fy_ok;

    // 判断是否存在字节序控制字符
    this->orderSymbol = (std::find(bo_bp, bo_ep, format[0]) != bo_ep)?(format[0]):(0x00);
    // 检测给出的格式符数量是否等于给出的参数数量
    if((expected_length = (this->orderSymbol)?(format.size() - 1):(format.size())) != args_length) {
        wmkcErr_exception(wmkcErr_Err32, "wmkcStruct::verifySymbol", std::string("pack expected " + \
            std::to_string(expected_length) + " items for packing (got " + std::to_string(args_length) + ")."));
    }

    // 检查所有格式符是否合法
    for(const auto &i : format) {
        if(!(fy_ok = (std::find(fy_bp, fy_ep, i) != fy_ep)) && !(bo_ok = (std::find(bo_bp, bo_ep, i) != bo_ep))) {
            wmkcErr_exception(wmkcErr_Err32, "wmkcStruct::verifySymbol", "bad char in struct format");
        }
    }
}

wmkcStruct::wmkcStruct()
: orderSymbol(), swapEndian(), bit16(), bit32(), bit64()
{

}

std::string wmkcStruct::pack(std::string format, std::vector<wmkcSize> args)
{
    if(format.empty()) {
        return std::string();
    }
    std::string result;
    wmkc_u32 index;

    this->verifySymbol(format, args.size()); // 检查格式符是否合法

    // 判断是否存在字节序控制字符
    if(this->orderSymbol) {
        // 判断指定的字节序是否与本机不同，如果是就将切换字节序的布尔值置为真
        if((WMKC_LE_DIAN && (this->orderSymbol == '>')) ||
            (WMKC_LE_DIAN && this->orderSymbol == '!') ||
            (!WMKC_LE_DIAN && (this->orderSymbol == '<'))) {
            this->swapEndian = true;
        }
    }




    std::cout << "everything ok." << std::endl;

    return result;
}

std::vector<wmkcSize> wmkcStruct::unpack(std::string format, std::string args)
{
    std::vector<wmkcSize> result;

    return result;
}

