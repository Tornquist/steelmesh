function initialize() {
    var source = new EventSource('/products/latest-product-events');
    source.addEventListener('message', function(e) {
        console.log("Received "+e.data);
        updateItemsPage(e.data);
    }, false);

    source.addEventListener('open', function(e) {
        console.log("Connection was opened.");
    }, false);

    source.addEventListener('error', function(e) {
        if (e.readyState == EventSource.CLOSED) {
            console.log("Connection was closed.");
        }else{
            console.log("Something else");
        }
    }, false);
};

function updateItemsPage(event) {
    var item = $('<li>').text(event);
    $('#items').prepend(item);
}

$(document).ready(initialize);
