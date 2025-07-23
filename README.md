Compile using the CMakeLists.txt.

CLI Format and Example:

    testbench.exe INPUT OUTPUT ENCODING
    testbench.exe module.txt tb.txt UTF16LE

The only encodings accepted are UTF8 and UTF16LE.
Most things use UTF8 but, pork2sausage/npp uses UTF16LE.
The output is always UTF8, the encoding is just for reading the input.

Verilog constructs not supported:

    "inout" ports
    "`define" constants 

If one of your input ports in "clk" or "clock", a #5 pulse will be created.