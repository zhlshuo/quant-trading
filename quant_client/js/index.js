$(document).ready(function(){

	/*
	* web socket for initilization
	*/
	var init_ws = new WebSocket("ws://localhost:9002");

	init_ws.onopen = function()
	{
	};

	init_ws.onmessage = function (evt)
	{
		var msg = JSON.parse(evt.data);

		if(msg.hasOwnProperty("deals"))
                {
			$('#deals_table tr').remove();

	                var deals = msg.deals;
        	        var deals_as_string = '';

                	for (i = 0; i < deals.length; i++) 
                        	deals_as_string += '<tr><td>' + deals[i].ticker + '</td><td>' + deals[i].quantity + '</td><td>' + deals[i].date + '</td><tr>';

        	        $('#deals_table tbody').append(deals_as_string);
		}


		if(msg.hasOwnProperty("tickers"))
	       	{
			var tickers = msg.tickers;
			var optionsAsString = "";
			for(var i = 0; i < tickers.length; i++) {
			    optionsAsString += "<option value='" + tickers[i] + "'>" + tickers[i] + "</option>";
			}

			$("#tickers_select").empty().append(optionsAsString);
		}

		if(msg.hasOwnProperty("books"))
                {
                        var books = msg.books;
			var trading_books = [];
			var customer_books = [];

			if(books.hasOwnProperty("trading_book"))
				trading_books = books.trading_book;
			
			if(books.hasOwnProperty("customer_book"))
                                customer_books = books.customer_book;
						
                        var trading_book_options = "";
                        for(var i = 0; i < trading_books.length; i++) {
                        	trading_book_options+= "<option value='" + trading_books[i].ID + "'>" + trading_books[i].Name + "</option>";
                        }
			
                        $("#trading_books_select").empty().append(trading_book_options);
			$("#risk_report_trading_books_select").empty().append(trading_book_options); // books for running risk report are the same with the trading books
			$("#trading_books_for_deals_select").empty().append(trading_book_options); // books to show deals

			var customer_book_options = "";
                        for(var i = 0; i < customer_books.length; i++) {
                                customer_book_options+= "<option value='" + customer_books[i].ID + "'>" + customer_books[i].Name + "</option>";
                        }

                        $("#customer_books_select").empty().append(customer_book_options);
                }		

		if(msg.hasOwnProperty("quotes"))
		{
			var ticker = msg.ticker;
			var quotes = msg.quotes;
			dateParts = quotes[0].date.split('-'),
			date = new Date(dateParts[0], parseInt(dateParts[1], 10) - 1, dateParts[2]);
			
			var data = [];
			for (i = 0; i < quotes.length; i++) {
				dateParts = quotes[i].date.split('-'); // date format is yyyy-mm-dd
	                        date = new Date(dateParts[0], parseInt(dateParts[1], 10) - 1, dateParts[2]);
                		data.push([
                    			date.getTime(),
                    			parseFloat(quotes[i].open) ,
                    			parseFloat(quotes[i].high) ,
                    			parseFloat(quotes[i].low) ,
                    			parseFloat(quotes[i].close)  
                    		]);
            		}

			CreateChart(ticker, data);	
		}
			
	};

	init_ws.onclose = function()
	{
	};

	$("#tickers_select").change(function(){
                init_ws.send('ticker_for_chart ' + $.trim($(this).find(":selected").text()));
        })

	$("#trading_books_for_deals_select").change(function(){
                init_ws.send('book_id_for_deals ' + $.trim($(this).find(":selected").val()));
        })


	function CreateChart(ticker, data) {
     		// create the chart
     		$("#price_chart").highcharts("StockChart", {
			title: {
				text: ticker + " Stock Price"
			},
         		rangeSelector: {
             			selected: 1
         		},
         		series: [{
             			type: "candlestick",
             			name: ticker + " Stock Price",
             			data: data,
				dataGrouping: {
					units: [
						["week", [1]],
						["month", [1,2,3,4,6]]
					]
				}
         		}]
     		});
 	}

	/*
	* web socket for trade booking
	*/
	var booking_ws = new WebSocket("ws://localhost:9003");

	booking_ws.onmessage = function (evt)
        {
		// if the trade book into the book that currently showing, book id sent to server and deals table get refresh
		if($('#trading_books_select').find(":selected").val() == $('#trading_books_for_deals_select').find(":selected").val())
			init_ws.send('book_id_for_deals ' + $.trim($("#trading_books_for_deals_select").find(":selected").val()));

		var n_trades_inserted = evt.data;
		$('#booking_feedback').text('trade inserted into database');
		$('#booking_feedback').show();
		$('#booking_feedback').fadeOut(2500);
	}

	$('#book_trade').click(function() {
		$('#quant_err_msg').hide();
		var quantity = $('#quantity').val();
		if (!isNumeric(quantity)) {
			$('#quant_err_msg').show();
			return false;
		}

		var trading_book = $('#trading_books_select').find(":selected").val();
		var customer_book = $('#customer_books_select').find(":selected").val();
		var ticker = $('#tickers_select').find(":selected").text();

		var today = new Date();
		var month = today.getMonth()+1;
		var day = today.getDate();

		var date = today.getFullYear() + '-' +
    		(month<10 ? '0' : '') + month + '-' +
    		(day<10 ? '0' : '') + day;

		booking_ws.send(trading_book + " " + customer_book + " " + ticker + " " + quantity + " " + date);
	});

	function isNumeric(n) {
		return !isNaN(parseFloat(n)) && isFinite(n);
	}


	/*
	* web socket for risk report
	*/
	var risk_report_ws = new WebSocket("ws://localhost:9004");

        risk_report_ws.onmessage = function (evt)
        {
		var msg = JSON.parse(evt.data);
                if(msg.hasOwnProperty("risk_reports"))
                {
                        var risk_reports = msg.risk_reports;
                        var optionsAsString = "";
                        for(var i = 0; i < risk_reports.length; i++) {
                            optionsAsString += "<option value='" + risk_reports[i] + "'>" + risk_reports[i] + "</option>";
                        }

                        $("#risk_report_select").empty().append(optionsAsString);
                }
		else if(msg.hasOwnProperty("error_msg"))
		{
			$('#report_output').val(msg.error_msg);
		}
		else
		{
			var report_output = '';
			$.each(msg, function(key, value){
   		 		report_output += key + ': ' + value + '\n';
			});
			$('#report_output').val(report_output);
		}

        }

        $('#run_risk_report').click(function() {

		var risk_report = $('#risk_report_select').find(":selected").val();
                var risk_report_trading_book = $('#risk_report_trading_books_select').find(":selected").val();

                risk_report_ws.send(risk_report + " " + risk_report_trading_book);
        });

	$("#msg_input").keyup(function(e) {
        	var code = e.keyCode ? e.keyCode : e.which;
        	if (code == 13) {  // Enter keycode
                	init_ws.send($('#msg_input').val());
                	$('#msg_input').val('');
        	}
	});
});

