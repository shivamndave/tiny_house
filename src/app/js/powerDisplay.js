// Power display JSON Chart
function setup_power() {
		// Create the chart
		$('#powerstats').highcharts('StockChart', {
         plotOptions : {
            line : {
               marker : {
                  enabled : false
               }
            }
         },
         chart: {
            width : 300,
            height : 100
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
           text : 'Power'
        },
        xAxis : {
         type: 'time'
      },
      yAxis : {
         title: {
            text: 'Watts'
         }
      },
      series : [{
        name : 'power watts',
        data : [[]],
        tooltip: {
          valueDecimals: 2,
          valueSuffix: ' W',
       }
    }]
 });
batteryStatus = $('#powerstatus').highcharts();
}
