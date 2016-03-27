// Power display JSON Chart
function setup_setpoint_chart(jqDOM, dataType, setpointValue) {
      var dataSensorInfo = dataType.sensor_info,
          dataEquipment = dataType.equipment,
          dataSensorValues = [[]];

          if(dataType.values.sensor.length > 0){
             dataSensorValues = dataType.values.sensor;
          }
          console.log(dataSensorValues);

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
            height : 300
         },
         rangeSelector : {
            enabled : true,
            selected : 4,
            buttons: [
            {
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
            },
            {
	       type: 'all',
	       text: 'All'
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
	       value: setpointValue,
               color: 'red',
               width: 2,
               id: 'plot-line-1',
               color: 'red',
               dashStyle: 'shortdash',
	       label: {
                  text: 'Setpoint'
               }
          }]
         },
         series : [{
            name : dataSensorInfo.longunit,
            data : dataSensorValues,
            tooltip: {
               valueDecimals: 2,
               valueSuffix: " " + dataSensorInfo.unit,
            }
         }]
      });
   return $(jqDOM).highcharts();
}
