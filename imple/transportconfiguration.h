


extern    enum bc_channel getTunnelOver();
extern    enum bc_channel getTunnelBelow();
extern    uint32 getResendTimeout();
extern    uint32 getRetryLimit();
extern    uint32 getTShy();
extern    uint32 getTConf();
extern    uint16 getConfLimit();
extern    uint8 getWindowSize();
extern    bool getUseCRC();
extern    bool getUseLinkEstablishmentProtocol();
extern  void setConfig ( uint32 aResendTimeout , uint32 aRetryLimit , 
                  uint32 aTShy , uint32 aTConf , uint16 aConfLimit , 
                  uint8 aWindowSize , bool aUseLinkEstablishmentProtocol ) ; 