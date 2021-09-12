module AutoVendor( give, change, totalMoney, clk, inputMoney, reset, choose ) ;
input clk, reset ;
input [7:0] inputMoney ; // User enter money
input [3:0] choose ; // Choose the drink
output [3:0] give ; // The drink finally get
output [7:0] change ; // Make change
output [7:0] totalMoney ; // Total money

reg [1:0] tea, coke, coffee, milk ; // Decide which drink
parameter S0 = 0 ; // State 0
parameter  S1 = 1 ; // State 1
parameter  S2 = 2 ; // State 2
parameter  S3 = 3 ; // State 3
	  
reg [2:0] state ;
reg [2:0] next_state ;

reg [7:0] totalMoney ;
reg [7:0] change ;
reg [3:0] give ;

initial 
begin
    totalMoney = 0 ;
    tea = 0 ;
    coke = 0 ;
    coffee = 0 ;
    milk = 0 ;
    change = 0 ;
    give = 0 ;
end

always @ ( posedge clk or reset ) begin // 決定是否Reset
  if ( reset ) begin
	totalMoney = 0 ;
    tea = 0 ;
    coke = 0 ;
    coffee = 0 ;
    milk = 0 ;
    change = 0 ;
    give = 0 ;
    state = S0 ;
  end
  else begin
    state = next_state ;
  end
end

always @ ( state or inputMoney or choose or posedge clk ) begin // 在某一個State的時候要做什麼事

case ( state )
    S0 :
    begin
		change = 0 ;
		give = 0 ;
		if ( !reset && inputMoney != 2 ) begin
			totalMoney = totalMoney + inputMoney ; // 總金額會是輸入的金額 
			if ( inputMoney != 0 && totalMoney != 0 )$display("coin %d, total %d dollars",inputMoney,totalMoney) ;
		end
		if ( totalMoney >= 10 && totalMoney < 15 && inputMoney != 2 && inputMoney != 0 ) $display("  tea") ;
		else if ( totalMoney >= 15 && totalMoney < 20 && inputMoney != 2 && inputMoney != 0 ) $display("  tea|coke") ;
		else if ( totalMoney >= 20 && totalMoney < 25 && inputMoney != 2 && inputMoney != 0 ) $display("  tea|coke|coffee") ;
		else if ( totalMoney >= 25 && inputMoney != 2 && inputMoney != 0 ) $display("  tea|coke|coffee|milk") ;
		else if ( totalMoney > 0 && totalMoney < 10 && inputMoney != 2 && inputMoney != 0 ) $display("  Nothing") ;
    end
    S1 :  
    begin
		if ( inputMoney != 0 ) begin
			totalMoney = totalMoney + inputMoney ; // 總金額會是輸入的金額 
			$display("coin %d, total %d dollars",inputMoney,totalMoney) ;
		end
		
        if ( totalMoney >= 10 && totalMoney < 15 ) begin // 只有tea可以喝
            tea = 1 ;
            coke = 0 ;
            milk = 0 ;
            coffee = 0 ;
        end
        else if ( totalMoney >= 15 && totalMoney < 20 ) begin // tea和coke都可以喝
            tea = 1 ;
            coke = 1 ;
			coffee = 0 ;
            milk = 0 ;
        end
        else if ( totalMoney >= 20 && totalMoney < 25 ) begin // tea、coke和coffee可以喝
            tea = 1 ;
            coke = 1 ;
			coffee = 1 ;
            milk = 0 ;
        end
        else if ( totalMoney >= 25 ) begin // 全部飲料都可以喝
            tea = 1 ;
            coke = 1 ;
			coffee = 1 ;
            milk = 1 ; 
        end
		else begin // 沒有飲料可以喝
			tea = 0 ;
            coke = 0 ;
            milk = 0 ;
            coffee = 0 ;
		end
    end

    S2 : 
    begin
		if ( choose == 1 && totalMoney >= 10 ) begin
				give = choose ;
				$display("tea out") ;
		end
		
		else if ( choose == 2 && totalMoney >= 15 ) begin
				give = choose ;
				$display("coke out") ;
		end
		
		else if ( choose == 3 && totalMoney >= 20 ) begin
				give = choose ;
				$display("coffee out") ;
		end
		
		else if ( choose == 4 && totalMoney >= 25 ) begin
				give = choose ;
				$display("milk out") ;
		end
		
		else begin
			give = 0 ;
        end
    end	
	
    S3 :
    begin

        if ( choose == 1 && totalMoney >= 10 ) change = totalMoney -10;
		else if ( choose == 2 && totalMoney >= 15 ) change = totalMoney -15;
		else if ( choose == 3 && totalMoney >= 20 ) change = totalMoney -20;
		else if ( choose == 4 && totalMoney >= 25 ) change = totalMoney -25;
		else change = totalMoney ;
		$display("exchange %d dollars",change) ;
		totalMoney = 0 ;
    end	
	
endcase
end

always @ ( posedge clk ) begin // 負責State間的變化
        case ( state )
        S0 :       
        begin
			//$display("In S0") ;
			if ( inputMoney == 2 ) begin
				next_state = S3 ;
			end
            else if ( totalMoney >= 10 ) begin
                next_state = S1 ;
            end
            else begin
                next_state = S0 ;
            end
        end
        S1 :
        begin
			//$display("In S1") ;
            if ( choose == 1 && inputMoney == 0 && totalMoney >= 10 ) begin
				if ( totalMoney >= 10 )
					next_state = S2;
				else
                    next_state = S0 ;
			end
		
			else if ( choose == 2 && inputMoney == 0 && totalMoney >= 15 ) begin
				if ( totalMoney >= 15 )
					next_state = S2;
				else
					next_state = S0 ;
			end
				 
			else if ( choose == 3 && inputMoney == 0 && totalMoney >= 20 ) begin
				if ( totalMoney >= 20 )
					next_state = S2;
				else
					next_state = S0 ;
			end
			
			else if ( choose == 4 && inputMoney == 0 && totalMoney >= 25 ) begin
				if ( totalMoney >= 25 )
					next_state = S2;
				else
					next_state = S0 ;
			end
			else if ( inputMoney == 0 ) begin
				next_state = S2 ;
			end
        end	  
		
        S2 :       
        begin
			//$display("In S2") ;
            next_state = S3 ;
        end		
		
		S3 :
		begin
			//$display("In S3") ;
		    next_state = S0 ; 
		end
		endcase
end

endmodule




