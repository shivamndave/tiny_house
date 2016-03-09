// Power display JSON Chart
function setup_setpoint_chart(jqDOM, dataType, setpoint) {
      var dataSensorInfo = dataType.sensor_info,
          dataEquipment = dataType.equipment;

      // Create the chart
      $(jqDOM).highcharts('StockChart', {
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
            text : dataSensorInfo.name
         },
         subtitle: {
             text: dataEquipment.name
         },
         xAxis : {
            type: 'datetime'
         },
         yAxis : {
            title: {
               text: dataSensorInfo.longunit
            },
            plotLines: [{
              value: setpoint,
              color: 'red',
              dashStyle: 'shortdash',
              width: 2,
              label: {
                  text: 'Setpoint'
              }
          }]
         },
         series : [{
            name : dataSensorInfo.longunit,
            data : [[]],
            tooltip: {
               valueDecimals: 2,
               valueSuffix: " " + dataSensorInfo.unit,
            }
         }]
      });
   return $(jqDOM).highcharts();
}
