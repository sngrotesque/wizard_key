#include <network/wmkc_tp.c>

int main(int argc, char **argv)
{
    wmkcNet_obj *net = wmkcNull;
    wmkcNet_new(&net, wmkcNull, AF_INET, false);
    wmkcNet_init(net, "0.0.0.0", WMKC_TP_DEFAULT_PORT);








    wmkcNet_close(net);
    wmkcNet_free(&net);
}
