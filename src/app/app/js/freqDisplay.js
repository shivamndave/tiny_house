// Power display JSON Chart
function setup_freq() {
      // Create the chart
      $('#freqstatus').highcharts('StockChart', {
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
               text: '1s'
            }, {
               type: 'minute',
               count: 1,
               text: '1m'
            }, {
               type: 'hour',
               count: 3,
               text: '1h'
            }, {
               type: 'day',
               count: 1,
               text: '1d'
            }
            ]
         },
         title : {
            text : 'Frequency'
         },
         xAxis : {
            type: 'datetime'
         },
         yAxis : {
            title: {
               text: 'Hertz'
            }
         },
         series : [{
            name : 'Frequency Hertz',
            data : [[]],
            tooltip: {
               valueDecimals: 2,
               valueSuffix: ' Hz',
            }
         }]
      });
freqStatus = $('#freqstatus').highcharts();
}
