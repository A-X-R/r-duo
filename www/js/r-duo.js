var first_focus = true;
var hold = true;
var mousedown = false;
var keydown = false;

function sendDirection(direction) {
 console.log("Sending ajax request direction: "+direction);
 mousedown = true;
 return false;
}

function sendStop() {
 console.log("Sending ajax request direction: stop");
 mousedown = false;
 return false;
}

function autorun() {
 $('#speechinput').focusout(function(){
  if ( first_focus != true ) {
   if ( $(this).val() == '' )
   {
    $(this).val('Ask a question to a maker! Type here!');
    first_focus = true;
   }
  }
 });

 $('#speechinput').focusin(function(){
  if ( first_focus == true ) {
   $(this).val('');
   first_focus = false;
  }
 });

 $('#speak').on("click",function(){
  var url = "/speak/" + encodeURIComponent($('#speechinput').val());
  console.log("AJAX: " + url);
 });

 var eventSubscription="click";
 if ( hold )
 {
  eventSubscription="mousedown";

  $('#east, #northeast, #north, #northwest, #west').on("mouseout",function(){
   if ( mousedown ) {
    return sendStop();
   }
  });

  $('#east, #northeast, #north, #northwest, #west').on("mouseup",function(){
   return sendStop();
  });

 }

 $('#east').on(eventSubscription,function(){
  return sendDirection("east");
 });

 $('#northeast').on(eventSubscription,function(){
  return sendDirection("northeast");
 });

 $('#north').on(eventSubscription,function(){
  return sendDirection("north");
 });

 $('#northwest').on(eventSubscription,function(){
  return sendDirection("northwest");
 });

 $('#west').on(eventSubscription,function(){
  return sendDirection("west");
 });

 $(document).keydown(function(e) {
  if(e.which == 13) {
   console.log('You pressed enter!');
   keydown = true;
  }
  if(e.which == 97 || e.which == 65) {
   console.log('You pressed "a"!');
   keydown = true;
  }
  if (e.which == 115 || e.which == 83) {
   console.log('You pressed "s"!');
   keydown = true;
  }
  if (e.which == 100 || e.which == 68) {
   console.log('You pressed "d"!');
   keydown = true;
  }
  if (e.which == 119 || e.which == 87) {
   console.log('You pressed "w"!');
   keydown = true;
  }
 });
 $(document).keyup(function(e) {
   if ( keydown ) {
    console.log('You let go!');
    keydown = false
   }
 });
}

if (window.addEventListener) window.addEventListener("load", autorun, false);
else if (window.attachEvent) window.attachEvent("onload", autorun);
else window.onload = autorun;
