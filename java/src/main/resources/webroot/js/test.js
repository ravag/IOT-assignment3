
let systemStatus = "CONNECTED";

//Variabile che indica ogni quanti millisecondi si faccia un aggiornamento automatico
const interval = 1000;


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
    const p = document.querySelector("div.valori p");
    const url = "/api/data";

    
    try {
        const response = await failableFetch(url)
        if (!response.ok) {
            throw new Error("errore di connessione: " + response.status)
        }
        const json = await response.json();
        let text = "";
        json.forEach(e => {
            text += `[${e["value"]},${e["time"]},${e["place"]}]`
        });
        p.innerHTML = text;
        systemStatus = "CONNECTED";

        drawGraph(json);

    } catch (error) {
        systemStatus = "NOT AVAILABLE";
        console.log(error);
    }

    console.log(systemStatus);
} 

document.querySelector("button").addEventListener('click', async function() {
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
});    

//Funzione per attendere ms millisecondi
const delay = ms => new Promise(res => setTimeout(res,ms));

//Esegue il refresh dei dati ogni ms millisecondi
async function refresh(ms) {
    while (true) {
        await delay(ms);
        console.log("ciao");
        addValues();
    }    
}

refresh(interval);
