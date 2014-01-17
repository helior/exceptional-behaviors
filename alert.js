(function($){
  
  Drupal.behaviors.xbAlert = {
    attach: function(context, settings) {
      var config = settings.xb.alert;
      $(config.selectors.target, context).once('xb-alert', function() {
        console.log($(this).text());
      });
    }
  }
  
})(jQuery);