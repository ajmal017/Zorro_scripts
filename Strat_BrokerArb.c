// Broker arbitrage example ////////////////////////////
// you can compare currency or CFD prices between broker A and broker B ...
// ... and enter a long position with the cheaper broker and a short position with the other

// There is special account list and asset list

#define ASSET_A "EURUSD_A"
#define ASSET_B "EURUSD_B"

function tick()
{
	asset(ASSET_A);
	var SpreadA = Spread, PriceA = priceClose(), 
		CommissionA = Commission*LotAmount/10000*PIP/PIPCost; // convert commission to price difference	
	asset(ASSET_B);
	var SpreadB = Spread, PriceB = priceClose(), 
		CommissionB = Commission*LotAmount/10000*PIP/PIPCost;

	var Threshold = 1.5*(SpreadA+SpreadB+CommissionA+CommissionB); // arbitrage threshold
	var Difference = PriceA - PriceB;
	printf("\n[%s.%.0f]  A %.5f  B %.5f",
		strdate(HMS,0),1000.*modf(second(),0),PriceA,PriceB);

	asset(ASSET_A);
	if(NumOpenShort && Difference < 0) exitShort();
	else if(NumOpenLong && Difference > 0) exitLong();
	else if(!NumOpenShort && Difference > Threshold) enterShort(); // go short with the expensive asset
	else if(!NumOpenLong && Difference < -Threshold) enterLong(); // go long with the cheap asset
		
	asset(ASSET_B);
	if(NumOpenShort && Difference > 0) exitShort();
	else if(NumOpenLong && Difference < 0) exitLong();		
	else if(!NumOpenShort && Difference < -Threshold) enterShort();
	else if(!NumOpenLong && Difference > Threshold) enterLong();
}

function run()
{
	if(!require(-2.1)) return;
	StartDate = EndDate = 2018;
	LookBack = 0;
	set(TICKS|LOGFILE);
	History = ".t1";
	assetList("AssetsArb.csv");
	asset(ASSET_A);
	asset(ASSET_B);
}
