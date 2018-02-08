#pragma once
#include "lwip/err.h"
#pragma clang diagnostic ignored "-Wpadded"
#include "lwip/netif.h"
#pragma clang diagnostic warning "-Wpadded"


err_t ethernetif_init(struct netif *netif);
void ethernetif_input(struct netif *netif);
void ethernetif_set_link(struct netif *netif);
void ethernetif_update_config(struct netif *netif);
void ethernetif_notify_conn_changed(struct netif *netif);

#ifdef __cplusplus
extern "C" {
#endif
u32_t sys_now();
#ifdef __cplusplus
}
#endif

extern uint gEthTimeLastEthifInput;
