%module ctrader
%{
#include "CTrader.h"
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcTraderApi.h"
#include "TraderSpi.h"
%}
%include "std_vector.i"
%include "ThostFtdcUserApiDataType.h"
%include "ThostFtdcUserApiStruct.h"
%include "ThostFtdcTraderApi.h"
%include "TraderSpi.h"
%include "CTrader.h"

