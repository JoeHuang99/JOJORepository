module Simulate() ;

reg clk, reset ;
reg [7:0] inputMoney ;
reg [3:0]  choose ;
wire [7:0] change ;
wire [3:0] give ; 
wire [7:0] totalMoney ;

AutoVendor autoVendor( give, change, totalMoney, clk, inputMoney, reset, choose ) ;

initial clk = 1'b1;
always #5 clk = ~clk;

initial
begin
/*
// 測試0
 reset = 1;
 choose = 0;
 inputMoney = 0 ;
 # 10 reset = 0;
 # 10 inputMoney = 10;
 # 10 inputMoney = 5;
 # 10 inputMoney = 1;
 # 10 inputMoney = 10;
 # 10 inputMoney = 0;
 
 # 10 choose = 3;
 # 30 choose = 0;
 */

// 測試1
 reset = 1;
 choose = 0;
 inputMoney = 0 ;
 # 10 reset = 0;
 # 10 inputMoney = 10;
 # 10 inputMoney = 1;
 # 10 inputMoney = 10;
 # 10 inputMoney = 0;

 # 10 choose = 3; // coffee
 # 30 choose = 0;

// 測試2
/*
reset = 1 ;
choose = 0;
inputMoney = 0 ;

# 10 reset = 0;
# 10 inputMoney = 5; // 投入五元
# 10 inputMoney = 10; // 投入十元
# 10 inputMoney = 2; // 取消

# 20 inputMoney = 10; // 投入十元
# 10 inputMoney = 10; // 投入十元
# 10 inputMoney = 1; // 投入一元
# 10 inputMoney = 1; // 投入一元
# 10 inputMoney = 1; // 投入一元
# 10 inputMoney = 1; // 投入一元
# 10 inputMoney = 1; // 投入一元
# 10 inputMoney = 0; // 確定不投錢

# 10 choose = 4; // 選擇購買牛奶
# 30 choose = 0; // 等待處理
*/
// 測試3
/*
reset = 1 ;
choose = 0;
inputMoney = 0 ;


# 10 reset = 0;
# 10 inputMoney = 5; // 投入五元
# 10 inputMoney = 10; // 投入十元
# 10 inputMoney = 2; // 取消
# 20 inputMoney = 10; // 投入十元
# 10 inputMoney = 10; // 投入十元
# 10 inputMoney = 0; // 確定不投錢
# 10 choose = 2; // 選擇購買可樂
# 30 choose = 0; // 等待處理
# 10 inputMoney = 10; // 投入十元
# 10 inputMoney = 10; // 投入十元
# 10 inputMoney = 0; // 確定不投錢
# 10 choose = 4; // 選擇購買牛奶
# 30 choose = 0; // 等待處理
# 10 inputMoney = 10; // 投入十元
# 10 inputMoney = 0; // 確定不投錢
# 10 choose = 1; // 選擇購買茶
# 30 choose = 0; // 等待處理
# 10 inputMoney = 50; // 投入五十元
# 10 inputMoney = 0; // 確定不投錢
# 10 choose = 3; // 選擇購買咖啡
# 30 choose = 0; // 等待處理
*/
end

endmodule
