var homeController = angular.module('appController'),
    GETURL = "api/history.php?",
    LIVEURL = "api/live.php?",
    NEWURL = "api/sensors",
    ROOMURL = "api/rooms",
    hideList = [],
    showList = [],
    init;

homeController.service('urlParser', function() {
    this.getLive = function(id, latest) {
        var tempURL = LIVEURL + "live=1&info_id=" + id.toString();
        if (latest > 5) {
            tempURL += "&latest=" + latest.toString();
        }

        return NEWURL;
    }

    this.getHist = function(id, latest) {
        var tempURL = GETURL,
            parDay = day.slice(0, 10),
            parStart = getTime(start),
            parEnd = getTime(end);

        console.log("day: " + parDay + " | str time: " + parStart + " | end time: " + parEnd);
        if (day && parStart && parEnd) {
            tempURL += "&day=" + parDay + "&start=" + parStart + "&end=" + parEnd;
            console.log("API call URL: " + tempURL);
        }

        return tempURL
    }

    // Gets only the time from the timepicker
    // and formats it from 00:11:22 into 00h11m22s (history.php usage)
    function getTime(time) {
        var tempTime;
        tempTime = replaceAt(time, 2, "h");
        tempTime = replaceAt(tempTime, 5, "m");
        tempTime += "s";
        return tempTime;
    }

    // Replaces at a specified character
    function replaceAt(str, ind, charac) {
        return str.substr(0, ind) + charac + str.substr(ind + charac.length);
    }
});


homeController.service('getDataService', function($http, urlParser) {
    return getDataMethod;

    function getDataMethod() {
        var request = $http({
            method: 'GET',
            url: urlParser.getLive(0, 0)
        }).then(function successCallback(response) {
            console.log("data")
            console.log(response.data)

            return response.data.all;
        });

        return request;
    }
});

homeController.service('charter', function() {});

homeController.controller('HomeController', function($scope, getDataService) {
    init = true;
    hideList = [];
    showList = [];
    console.log("data");

    getDataService().then(function(data) {
        console.log(data);
        $scope.init_data_array = data;
    });
});

homeController.directive('mgDisplay', function($interval, getDataService, charter) {
    return {
        restrict: 'E',
        scope: {
            display: '@',
            ind: '@',
            status: '@'
        },
        link: link,
        templateUrl: 'static/home/templates/display.html'
    }

    function link(scope, element, attrs) {
        scope.$watch(attrs.display, function(data) {
            console.log(data);
            scope.sensorinfo_name = data.sensor_info.name;
            scope.sensorinfo_info = data.sensor_info.info;
            scope.equipment_name = data.equipment.name;
            scope.equipment_location = data.equipment.location;
            scope.equipment_info = data.equipment.info;

            scope.latest_time = "Loading latest time...";
            scope.latest_status = "Loading latest status...";

            console.log("attrs.ind")
            console.log(attrs.ind)
            scope.chart_id = angular.element("#"+String(attrs.ind));
            scope.status = scope.chart_id.highcharts('StockChart', {
                plotOptions: {
                    line: {
                        marker: {
                            enabled: false
                        }
                    }
                },
                chart: {
                    height: 300
                },
                rangeSelector: {
                    enabled: true,
                    selected: 4,
                    buttons: [{
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
                    }, {
                        type: 'all',
                        text: 'All'
                    }]
                },
                title: {
                    text: data.sensor_info.name
                },
                subtitle: {
                    text: data.equipment.name
                },
                xAxis: {
                    type: 'datetime'
                },
                yAxis: {
                    title: {
                        text: data.sensor_info.longunit
                    }
                },
                series: [{
                    name: data.sensor_info.longunit,
                    data: data.values.sensor,
                    tooltip: {
                        valueDecimals: 2,
                        valueSuffix: " " + data.sensor_info.unit,
                    }
                }]
            });
            console.log(scope.status);
            $interval(function() {
                getDataService().then(function(tempData) {
                    var currentData = tempData[attrs.ind];
                    console.logcurrentData
                    console.log(scope.chart_id.highcharts().series[0]);
                    scope.chart_id.highcharts().series[0].setData(currentData.values.sensor, true);

                    var latestTimeVal = currentData.values.sensor[currentData.values.sensor.length - 1][0],
                    latestTimeConv = new Date(latestTimeVal);
                    scope.latest_time = "Latest time: " + String(latestTimeConv);

                    var latestStatVal = currentData.sensor_info.status,
                    statusText = "";

                    if(latestStatVal == 0) {
                        statusText = "OFF";
                    } else {
                        statusText = "ON";
                    }
                    scope.latest_status = "Latest status: " + statusText;
                });
            }, 5000)
        });
    }
});
