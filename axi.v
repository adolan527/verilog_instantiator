/****************************************/
/****************axi****************/
/****************************************/

/**************** Inputs ****************/
reg clk_in, reset_in, n, write_enable, read_enable, s_axi_awready, s_axi_wready, s_axi_bvalid, s_axi_arready, s_axi_rvalid;

/**************** Outputs ****************/
wire write_response_valid read_response_valid s_axi_aclk s_axi_sreset s_axi_awvalid s_axi_wvalid s_axi_bready s_axi_arvalid s_axi_rready;

/**************** Inouts ****************/


axi axi_inst(
	.clk_in(clk_in),								//Input[0:0] clk_in //
	.reset_in(reset_in),							//Input[0:0] reset_in //
	.n(n),											//Input[0:0] n //
	.write_enable(write_enable),					//Input[0:0] write_enable //// this is the write enable port

	.write_response_valid(write_response_valid),	//Output[0:0] write_response_valid //
	.read_enable(read_enable),						//Input[0:0] read_enable //
	.read_response_valid(read_response_valid),		//Output[0:0] read_response_valid //
	.s_axi_aclk(s_axi_aclk),						//Output[0:0] s_axi_aclk //
	.s_axi_sreset(s_axi_sreset),					//Output[0:0] s_axi_sreset //
	.s_axi_awvalid(s_axi_awvalid),					//Output[0:0] s_axi_awvalid //
	.s_axi_awready(s_axi_awready),					//Input[0:0] s_axi_awready //
	.s_axi_wvalid(s_axi_wvalid),					//Output[0:0] s_axi_wvalid //
	.s_axi_wready(s_axi_wready),					//Input[0:0] s_axi_wready //
	.s_axi_bvalid(s_axi_bvalid),					//Input[0:0] s_axi_bvalid //
	.s_axi_bready(s_axi_bready),					//Output[0:0] s_axi_bready //
	.s_axi_arvalid(s_axi_arvalid),					//Output[0:0] s_axi_arvalid //
	.s_axi_arready(s_axi_arready),					//Input[0:0] s_axi_arready //
	.s_axi_rvalid(s_axi_rvalid),					//Input[0:0] s_axi_rvalid //
	.s_axi_rready(s_axi_rready) 					//Output[0:0] s_axi_rready //
);



