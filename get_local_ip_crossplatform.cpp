#ifdef linux
#include <ifaddrs.h>
#include <linux/if_packet.h>
#else ifdef WIN32
#include <iphlpapi.h>
#endif

bool ComputerMacVerifier::GetLocalIP(std::string &mac)
{
#ifdef linux
    struct ifaddrs *iflist, *iface;

    if (getifaddrs(&iflist) < 0)
    {
#ifdef ENABLE_ENDECRYPT_LOG
        std::cout << "Get local Mac failed!" << std::endl;
#endif
        return false;
    }

    for (iface = iflist; iface; iface = iface->ifa_next)
    {
        if (strstr(iface->ifa_name, "eth") != NULL &&
                iface->ifa_addr->sa_family == AF_PACKET)
        {
            sockaddr_ll *s = (struct sockaddr_ll*)iface->ifa_addr;
            mac = ConvertMac2String(s->sll_addr);
        }
    }
    freeifaddrs(iflist);
#else ifdef WIN32
    PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO[1];
    PIP_ADAPTER_INFO pCurrAdapterInfo;
    unsigned long stSize = sizeof(IP_ADAPTER_INFO);
    int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
    if (ERROR_BUFFER_OVERFLOW == nRel)
    {
        delete[] pIpAdapterInfo;
        pIpAdapterInfo = new IP_ADAPTER_INFO[stSize/sizeof(IP_ADAPTER_INFO)];
        nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);
    }

    pCurrAdapterInfo = pIpAdapterInfo;
    if (nRel == ERROR_SUCCESS)
    {
        if(pCurrAdapterInfo->Type == MIB_IF_TYPE_ETHERNET)
        {
            for (DWORD i = 0; i < pCurrAdapterInfo->AddressLength; i++)
            {
                IP_ADDR_STRING *pIpAddrString =&(pCurrAdapterInfo->IpAddressList);
                do
                {
                    mac = (char*)&pIpAddrString->IpAddress;
                    pIpAddrString=pIpAddrString->Next;
                } while (pIpAddrString);

            }
        }
        pCurrAdapterInfo = pCurrAdapterInfo->Next;
    }

    if (pIpAdapterInfo)
    {
        delete[] pIpAdapterInfo;
    }
#endif
    return true;
}
