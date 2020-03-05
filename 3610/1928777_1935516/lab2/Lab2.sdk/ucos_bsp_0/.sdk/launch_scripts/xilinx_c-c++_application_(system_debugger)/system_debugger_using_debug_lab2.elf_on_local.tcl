connect -url tcp:127.0.0.1:3121
source C:/TEMP/INF3610/3610/1928777_1935516/lab2/Lab2.sdk/ucos_bsp_0/design_1_wrapper_hw_platform_0/ps7_init.tcl
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent Zed 210248685962"} -index 0
loadhw -hw C:/TEMP/INF3610/3610/1928777_1935516/lab2/Lab2.sdk/ucos_bsp_0/design_1_wrapper_hw_platform_0/system.hdf -mem-ranges [list {0x40000000 0xbfffffff}]
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent Zed 210248685962"} -index 0
stop
ps7_init
ps7_post_config
configparams force-mem-access 0
