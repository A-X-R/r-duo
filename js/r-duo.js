var first_focus = true;
var hold = true;
var down = false;
 
function sendDirection(direction) {
 console.log("Sending ajax request direction: "+direction);
 down = true;
 return false;
}

function sendStop() {
 console.log("Sending ajax request direction: stop");
 down = false;
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
   if ( down ) {
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
}

if (window.addEventListener) window.addEventListener("load", autorun, false);
else if (window.attachEvent) window.attachEvent("onload", autorun);
else window.onload = autorun;
