
let systemStatus = "AUTOMATIC";

const switch_mode_button = document.getElementById("switch_mode");
const open_slider = document.getElementById("opening");
const openingText = document.getElementById("openingText");
const statusText = document.getElementById("statusText");
const setOpeningBtn = document.getElementById("setOpening")

setOpeningBtn.addEventListener("click",async function(event){
    event.preventDefault();
    const url = "/api/data";

    try {
        const response = await fetch(url, {
            method: "POST",
            body: `{"open":${open_slider.value}}`
        });
        if (!response.ok) {
            throw new Error("errore di connessione: " + response.status);
        }
    } catch (error) {
        console.log(error);
    }

})
//Variabile che indica ogni quanti millisecondi si faccia un aggiornamento automatico
const interval = 5000;

switch_mode_button.addEventListener("click",async function(event) {
    event.preventDefault();
    let status = systemStatus == "AUTOMATIC" ? "MANUAL" : "AUTOMATIC";
    const url = "/api/mode";

    try {
        const response = await fetch(url, {
            method: "POST",
            headers: {
                "Content-type":"application/json"
            },
            body: `{"state":"${status}"}`
        });
        if (!response.ok) {
            throw new Error("errore di connessione: " + response.status)
        }
        const json = await response.json();
        systemStatus = json["state"];
    } catch (error) {
        console.log(error);
        systemStatus = "NOT AVAILABLE";
    }
    buttonHandler();
});

//2 modi per fare grafici, bisogna decidere, io preferisco usare Ploty
//Data must be a json array
function drawGraph(data) {
    //Grafico fatto con chart.js
    const div = document.getElementById("chartArea");
    div.innerHTML = '<canvas id="waterGraph" style="width: 100%;"></canvas>'
    const graph = document.getElementById("waterGraph");

    const xValues = new Array();
    const yValues = new Array();

    data.forEach(e => {
        yValues.push(e["value"]);
        xValues.push(e["time"]);
    });

    if (interval >= 5000) {
        new Chart(graph, {
            type: "line",
            data: {
                labels: xValues,
                datasets: [{
                    fill: true,
                    lineTension: 0,
                    backgroundColor: "rgba(0,0,255,1.0)",
                    borderColor: "rgba(0,0,255,1.0)",
                    data: yValues
                }]
            },
            options: {
                plugins: {
                    legend: {display:false},
                    title: {
                        display: true,
                        text: "Water level",
                        font: {size:16}
                    },
                    filler: {
                        drawTime: "beforeDatasetDraw"
                    }
                }
            }
        });
    } else {
        //Grafico fatto con Ploty 
        // Define Data
        const inputs = [{
            x: xValues,
            y: yValues,
            mode:"lines"
        }];

        // Define Layout
        const layout = {
            xaxis: {range: [xValues[0], xValues[xValues.length -1 ]],title: "Time"},
            yaxis: {range: [0, 1], title: "Level"},  
            title: "Watah"
        };

        // Display using Plotly
        Plotly.newPlot("charticus", inputs, layout);
    }
}

//Una fecth che ha un limite di tempo per andare a buon fine
async function failableFetch(url) { 
    const response = await Promise.race([
        fetch(url),
        new Promise((_,reject) => 
            setTimeout(() => 
                reject(new Error("Timeout"))
        ,interval)),
    ]);
    return response;
}

//
async function addValues() {
    const url = "/api/data";

    try {
        const response = await failableFetch(url)
        if (!response.ok) {
            throw new Error("errore di connessione: " + response.status)
        }
        const json = await response.json();
        systemStatus = json["status"];
        //systemStatus = systemStatus=="NOT AVAILABLE"? "AUTOMATIC" : systemStatus; /*Prendi dal messaggio lo stato a cui riandare se prima era NOT AVAILABLE*/ 
        //console.log(json);
        openingText.innerHTML = json["opening"];
        drawGraph(json["data"]);

    } catch (error) {
        systemStatus = "NOT AVAILABLE";
        console.log(error);
    }
    
    console.log(systemStatus);
} 

open_slider.addEventListener("input",function(){
    num = document.getElementById("currentOpening");
    num.value = open_slider.value; 
})

/* document.querySelector("button").addEventListener('click', async function() {
    const url = "/api/data";
    const json = JSON.parse(`{"value":${Math.random()}, "time":${Date.now()}, "place":"caio"}`)

    console.log(json);

    try {
        const response = await fetch(url, {
            method: "POST",
            body: `{"value":${Math.random()}, "time":${Date.now()}, "place":"caio"}`
        });
        if (!response.ok) {
            throw new Error("errore di connessione: " + response.status)
        }

    } catch (error) {
        console.log(error);
    }
});    */ 

function buttonHandler() {
    statusText.innerHTML = systemStatus;
    switch (systemStatus) {
        case "UNCONNECTED":
            switch_mode_button.disabled = true;
            open_slider.disabled = true;
            setOpeningBtn.disabled = true;
            break;

        case "MANUAL":
            switch_mode_button.disabled = false;
            open_slider.disabled = false;
            setOpeningBtn.disabled = false;
            switch_mode_button.innerHTML = "AUTOMATIC";
            break;

        case "AUTOMATIC":
            switch_mode_button.disabled = false;
            open_slider.disabled = true;
            setOpeningBtn.disabled = true;
            switch_mode_button.innerHTML = "MANUAL";
            break;

        default:
            switch_mode_button.disabled = true;
            open_slider.disabled = true;
            setOpeningBtn.disabled = true;
            break;
    }
}

//Funzione per attendere ms millisecondi
const delay = ms => new Promise(res => setTimeout(res,ms));

//Esegue il refresh dei dati ogni ms millisecondi
async function refresh(ms) {
    while (true) {
        await delay(ms);
        addValues();

        buttonHandler();
    }    
}

refresh(interval);
