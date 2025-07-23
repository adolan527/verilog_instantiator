/****************************************/
/****************programCounter****************/
/****************************************/

/**************** Inputs ****************/
reg clk, reset_n, GatePC, enable;
reg[1:0] pcMux;
reg[15:0] bus, adder;

/**************** Outputs ****************/
wire[15:0] result;

/**************** Inouts ****************/


programCounter programCounter_inst(
	.pcMux(pcMux),									//Input[1:0] pcMux //
	.bus(bus),										//Input[15:0] bus //
	.adder(adder),									//Input[15:0] adder //
	.clk(clk),										//Input[0:0] clk //
	.reset_n(reset_n),								//Input[0:0] reset_n //
	.GatePC(GatePC),								//Input[0:0] GatePC //
	.enable(enable),								//Input[0:0] enable //
	.result(result) 								//Output[15:0] result //
);



