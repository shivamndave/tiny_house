// Power display JSON Chart
function setup_temp() {
		// Create the chart
		$('#tempstatus').highcharts('StockChart', {
         plotOptions : {
            line : {
               marker : {
                  enabled : false
               }
            }
         },
         chart: {
            width : 500,
            height : 300
         },
         rangeSelector : {
            enabled : true,
            selected : 0,
            buttons: [
            {
               type: 'second',
               count: 1,
               text: '1d'
            }, {
               type: 'minute',
               count: 1,
               text: '1m'
            }, {
               type: 'hour',
               count: 3,
               text: '3m'
            }, {
               type: 'day',
               count: 1,
               text: '1d'
            }
            ]
         },
         title : {
           text : 'Temperature'
        },
        xAxis : {
         type: 'datetime'
      },
      yAxis : {
         title: {
            text: 'Fahrenheit'
         }
      },
      series : [{
        name : 'Temperature Fahrenheit',
        data : [[]],
        tooltip: {
          valueDecimals: 2,
          valueSuffix: ' F',
       }
    }]
 });
tempStatus = $('#tempstatus').highcharts();
}
