const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<meta name="viewport" content="width=device-width,initial-scale=1.0">
<html>
<!-- HTML_STYLE -->
<style>
    body {
        text-align: center;
        font-family: helvetica;
        background-color: lightskyblue;
        margin: 0;
    }
    form {
        display: flex;
    }
    .spacer {
        display: block;
        width: 100%;
        height: 80px;
    }
    .header {
        display: flex;
        position: fixed;
        top: 0;
        width: 100%;
        background-color: white;
    }
    .container {
        background-color: whitesmoke;
        display: flex;
        flex-direction: column;
        justify-content: center;
        border: solid 1px lightgray;
        border-radius: 5px;
        width: 500px;
        margin: auto;
        margin-bottom: 10px;
    }
    .column {
        display: flex;
        flex-direction: column;
    }
    .setting-container {
        display: flex;
        flex-direction: column;
        border-bottom: 1px solid lightgray;
    }
    .setting-container form {
        flex-wrap: wrap;
        justify-content: flex-end;
        padding: 10px;
    }
    .setting-container label {
        display: flex;
        align-items: center;
        justify-content: flex-start;
        width: calc(35% - 10px);
        margin: 5px 0 5px;
        text-align: left;
    }
    .setting-container input[type='text'], input[type='checkbox'] {
        width: 65%;
        margin: 5px 0 5px;
        height: 30px;
        border: none;
        border-radius: 5px;
        padding-left: 9px;
    }
    #file-update-label {
        background-color: white;
        border-radius: 5px;
        padding: 5px;
        height: 30px;
        margin: auto;
        width: 70%;
    }
    .row {
        display: flex;
        flex-direction: row;
        align-items: center;
        justify-content: space-between;
        flex-wrap: wrap;
    }
    .hidden {
        display: none !important;
    }
    .header-container {
        display: flex;
        flex-direction: row;
        justify-content: space-between;
        flex-wrap: wrap;
        border-bottom: solid 1px lightgray;
    }
    .header-container > h2 {
        padding-left: 10px;
    }
    .row>.label {
        margin-left: 10px;
    }
    .set {
        display: flex;
        flex-direction: column;
        width: 100%;
        background-color: lightgray;
        padding: 10px;
    }
    .set>.set-inputs {
        display: flex;
        flex-direction: column;
    }
    .set>.set-inputs input {
        width: 50%;
        height: 30px;
        border: none;
        border-radius: 5px;
        padding-left: 10px;
        margin-top: 5px;
        margin-bottom: 5px;
    }
    .set>.set-inputs>.column {
        background: darkgrey;
        border-radius: 5px;
        padding: 10px;
        color: white;
        margin-top: 5px;
    }
    select {
        width: calc(50% + 10px);
        height: 30px;
        border: none;
        margin-top: 5px;
        margin-bottom: 5px;
    }
    .set>.set-buttons {
        display: flex;
        flex-direction: row;
        justify-content: flex-end;
    }
    .btn-container {
        display: flex;
        justify-content: flex-end;
    }
    .btn-container input {
        width: 15%;
        border: 1px solid lightgrey;
        border-radius: 5px;
        padding: 15px 32px;
        margin: 10px 5px;
    }
    .btn {
        border-radius: 5px;
        padding: 15px 32px;
        text-align: center;
        transition-duration: 0.4s;
        margin: 10px 5px;
        cursor: pointer;
        text-decoration: none;
    }
    .edit {
        background-color: dodgerblue;
        border: solid 1px dodgerblue;
        color: white;
    }
    .edit:hover {
        color: dodgerblue;
    }
    .on,.save {
        background-color: yellowgreen;
        border: solid 1px yellowgreen;
        color: white;
    }
    .on:hover,.save:hover {
        color: yellowgreen;
    }
    .off,.delete {
        background-color: tomato;
        border: solid 1px tomato;
        color: white;
    }
    .off:hover, .delete:hover {
        color: tomato;
    }
    .cancel {
        background-color: lightgray;
        border: solid 1px lightgray;
        color: white;
    }
    .cancel:hover {
        color: lightgray;
    }
    .btn:hover {
        background-color: white;
    }
    .input-mode {
        pointer-events: none;
        background-color: white;
    }
    .input-mode.on {
        color: yellowgreen;
    }
    .input-mode.off {
        color: tomato;
    }
    #blocker {
        display: flex;
        flex-direction: column;
        justify-content: center;
        align-items: center;
        top: 0;
        left: 0;
        z-index: 1;
        position: fixed;
        height: 100%;
        width: 100%;
        background: lightgray;
        opacity: 0.95;
        color: white;
    }
    .spinner {
        height: 30px;
        border-radius: 5px;
        background-color: lightgray;
    }
    .lds-ring {
        width: 50px;
        height: 50px;
        display: inline-block;
        position: relative;
        margin: 20px;
    }
    .lds-ring div {
        box-sizing: border-box;
        display: block;
        position: absolute;
        width: 100%;
        height: 100%;
        border: 8px solid #fff;
        border-radius: 50%;
        animation: lds-ring 1.2s cubic-bezier(0.5, 0, 0.5, 1) infinite;
        border-color: #fff transparent transparent transparent;
    }
    .lds-ring div:nth-child(1) {
        animation-delay: -0.45s;
    }
    .lds-ring div:nth-child(2) {
        animation-delay: -0.3s;
    }
    .lds-ring div:nth-child(3) {
        animation-delay: -0.15s;
    }
    @keyframes lds-ring {
        0% {
            transform: rotate(0deg);
        }
        100% {
            transform: rotate(360deg);
        }
    }
    @media only screen and (max-width: 600px) {
        body {
            margin: 0;
        }
        .container {
            width: 100%;
            border: none;
            border-radius: 0px;
        }
    }
    .ok {
        background-color: green;
    }
    .error {
        background-color: red;
    }
</style>
<!-- HTML_DOM -->
<body>
    <div class="spacer"></div>
    <div class="header">
        <a id='restart-button' onclick='restart()' class='btn edit'>Restart</a>
    </div>
    <div class="lds-ring" id='page-loader'><div></div><div></div><div></div><div></div></div>
    <div class='container' id="gpio-container">
        <div id='remote-header-bar' class="header-container">
            <h2>Install Project from Github repositories</h2>
        </div>
        <div id='remote' class='column'></div>
    </div>
    <div class="container" id="automation-container">
        <div id='local-header-bar' class="header-container">
            <h2>Install project from local directory</h2>
        </div>
        <div id='local' class='column'></div>
    </div>
    <div id='blocker' class='hidden'>
        <h2 id='blocker-title'>Loading</h2>
        <div class="lds-ring"><div></div><div></div><div></div><div></div></div>
    </div>
</body>
</html>
<!-- HTML_SCRIPT -->
<script>
    var repos = [];
    const delay = (ms => new Promise(resolve => setTimeout(resolve, ms)));
    // Restart ESP
    const restart = async () => {
        try {
            const res = await fetch(window.location.href + 'restart');
            blocker.classList.remove('hidden');
            await delay(2000);
            location.reload();
        } catch (err) {
            blocker.classList.add('hidden');
            console.error(`Error: ${err}`);
        }
    }

    const createRepoRow = (repo,index) => {
        let child = document.createElement('div');
        child.innerHTML = `<div class='row' id='rowRepo-${index}'>
            <div class='info'>
                <div>
                    ${repo.name}
                </div>
                <div>
                    ${repo.description}
                </div>
            </div>
            <div class='btn-container'>
                <a onclick='fetchRepoInfo(this)' id='getRepoInfo-${index}' class='btn edit'>More info</a>
            </div>
        </div>`;
        return child.firstChild;
    }

    // Fetch all project repo available
    const fetchRepoList = async () => {
        try {
            const res = await fetch(window.location.href + 'repo/list');
            repos = await res.json();
            const container = document.getElementById('remote');
            repos.forEach((repo,index) => {
                container.appendChild(createRepoRow(repo,index));
            });
        } catch (err) {
            console.error(`Error: ${err}`);
        }
    }

    const fetchRepoInfo = async (element) => {
        try {
            const repoIndex = element.id.split('-')[1];
            const repo = repos[repoIndex];
            const res = await fetch(window.location.href + 'repo/info', {
                method: 'post',
                headers: {
                    'Accept': 'application/json',
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({repo: repo.repoRawVersionList})
            });
            repoMoreInfor = await res.json();
            
        } catch (err) {
            console.error(`Error: ${err}`);
        }
    }

    // Events
    window.onload = async () => {
        await fetchRepoList();
        document.getElementById('page-loader').remove();
    };
</script>
<!-- HTML_SCRIPT_END -->
)=====";