#include "headers.h"
#include <pgmspace.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Aviário - Painel de Controle</title>
  <link rel="shortcut icon" type="image/x-icon" href="https://truemining.online/images/frango.webp" />
  <meta http-equiv="Content-Type" content="text/html; charset=utf8">
  <meta name="viewport" content="width=device-width, initial-scale=1; charset=utf8">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h3 {font-size: 1.8rem; color: white;}
    h4 { font-size: 1.2rem;}
    p { font-size: 1.4rem;}
    body {  margin: 0;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px; margin-bottom: 20px;}
    .switch input {display: none;}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 68px;   opacity: 0.8;   cursor: pointer;}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #1b78e2}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
    .topnav { overflow: hidden; background-color: #1b78e2;}
    .content { padding: 20px;}
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);}
    .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));}
    .slider2 { -webkit-appearance: none; margin: 14px;  height: 20px; background: #ccc; outline: none; opacity: 0.8; -webkit-transition: .2s; transition: opacity .2s; margin-bottom: 40px; }
    .slider:hover, .slider2:hover { opacity: 1; }
    .slider2::-webkit-slider-thumb { -webkit-appearance: none; appearance: none; width: 40px; height: 40px; background: #008B74; cursor: pointer; }
    .slider2::-moz-range-thumb { width: 40px; height: 40px; background: #008B74; cursor: pointer;}
    .reading { font-size: 2.6rem;}
    .card-switch {color: #50a2ff; }
    .card-light{ color: #008B74;}
    .card-bme{ color: #572dfb;}
    .card-motion{ color: #3b3b3b; cursor: pointer;}
    .icon-pointer{ cursor: pointer;}
    .center-v { line-height: 1.5; display: inline-block; vertical-align: middle; }
  </style>
</head>
<body>
  <div class="topnav">
    <h3>AVIÁRIO - PAINEL DE CONTROLE<span style="text-align:right;">&nbsp;&nbsp; <i class="fas fa-user-slash fa-xs center-v icon-pointer" onclick="logoutButton()"></i></span></h3>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card card-bme">
        <h4><i class="fas fa-thermometer-quarter"></i> TEMPERATURA</h4><div><p class="reading"><span id="temp"></span> &deg;C</p></div>
      </div>
      <div class="card card-bme">
        <h4><i class="fas fa-tint"></i> UMIDADE</h4><div><p class="reading"><span id="humi"></span> &percnt;</p></div>
      </div>
      <div class="card card-bme">
        <h4><i class="fas fa-wifi"></i> WIFI</h4><div><p class="reading"><span id="rssi"></span> dBi</p></div>
      </div>
    <!--  %BUTTONPLACEHOLDER%   -->   
  </div>
</div>
<h4><i class="far fa-clock icon-pointer" onClick="document.location.reload(true)"> Leitura: <span id="lastUpdatedTime"></span></h4></i>
%VERSIONLABELPLACEHOLDER%
<script>
function logoutButton() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/logout", true);
  xhr.send();
  setTimeout(function(){ window.open("/logged-out","_self"); }, 1000);
}
function controlVentMin(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/ventMin?state=1", true); }
  else { xhr.open("GET", "/ventMin?state=0", true); }
  xhr.send();
}
function toggleLed(element) {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/toggle", true);
  xhr.send();
}
function clearMotionAlert() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/clear-motion", true);
  xhr.send();
  setTimeout(function(){
    document.getElementById("motion").innerHTML = "No motion";
    document.getElementById("motion").style.color = "#3b3b3b";
  }, 1000);
}
function getPageData() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/get-data", true);
  xhr.send();
}
if (!!window.EventSource) {
 var source = new EventSource('/events');
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/get-data", true);
  xhr.send();
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 source.addEventListener('led_state', function(e) {
  console.log("led_state", e.data);
  var inputChecked;
  if( e.data == 1){ inputChecked = true; }
  else { inputChecked = false; }
  document.getElementById("led").checked = inputChecked;
 }, false);
 source.addEventListener('temperature', function(e) {
  console.log("temperature", e.data);
  document.getElementById("temp").innerHTML = e.data;
 }, false);
 source.addEventListener('humidity', function(e) {
  console.log("humidity", e.data);
  document.getElementById("humi").innerHTML = e.data;
 }, false);
  source.addEventListener('lastUpdatedTime', function(e) {
  console.log("lastUpdatedTime", e.data);
  document.getElementById("lastUpdatedTime").innerHTML = e.data;
 }, false);
 source.addEventListener('ventMinCkeckBox', function(e) {
 console.log("ventMinCkeckBox", e.data);
 if (e.data == "true")
 {
  document.getElementById("ventMinCkeckBox").checked = true;
 }
 else
 {
  document.getElementById("ventMinCkeckBox").checked = false;
 }
}, false);


source.addEventListener('rssi', function(e) {
console.log("rssi", e.data);
document.getElementById("rssi").innerHTML = e.data;
}, false);
}</script>
</body>
</html>)rawliteral";

const char logout_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <p>Logged out or <a href="/">return to homepage</a>.</p>
  <p><strong>Note:</strong> close all web browser tabs to complete the logout process.</p>
</body>
</html>
)rawliteral";