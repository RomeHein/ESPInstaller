const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<meta name="viewport" content="width=device-width,initial-scale=1.0">
<html>
<!-- HTML_STYLE -->
<style>
    #remote {
        max-height: 300px;
        overflow: scroll;
        background-color: whitesmoke;
    }
    #file-update-label {
        display: flex;
        justify-content: center;
        align-items: center;
        background-color: white;
        border-radius: 5px;
        padding: 5px;
        height: 30px;
        width: 70%;
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
    body {
        text-align: center;
        font-family: helvetica;
        background-color: steelblue;
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
    .general-info {
        max-height: 100px;
        width: 67%;
        display: flex;
        flex-direction: column;
        padding: 10px;
    }
    .general-info > .repo-name {
        text-align: left;
        font-size: larger;
        margin-bottom: 5px;
    }
    .general-info > .repo-description {
        font-size: smaller;
        text-align: left;
    }
    .more-info {
        background-color: lightgray !important;
        width: 100%;
    }
    .more-info .repo-info {
        width: 80%;
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
    .row {
        display: flex;
        flex-direction: row;
        align-items: center;
        justify-content: space-between;
        flex-wrap: wrap;
        background-color: white;
        margin: 5px;
        border-radius: 5px;
    }
    .set-inputs .row {
        background-color: initial;
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
        background-color: whitesmoke;
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
        padding: 15px 10px;
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
    .ok {
        background-color: green;
    }
    .error {
        background-color: red;
    }
    .cancel:hover {
        color: lightgray;
    }
    .btn:hover {
        background-color: white;
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
            <h2>Install from repositories</h2>
        </div>
        <div id='remote' class='column'></div>
    </div>
    <div class="container" id="automation-container">
        <div id='local-header-bar' class="header-container">
            <h2>Install from custom URL</h2>
        </div>
        <div id='local' class='column'>
            <div class='set'>
                <div class='set-inputs'>
                    <div class='row'>
                        <label for='spiff-path'>Spiff file URL:</label>
                        <input type='text' name='spiff' id='spiff-path-input'>
                    </div>
                    <div class='row'>
                        <label for='bin-path'>Bin file URL</label>
                        <input type='text' name='bin' id='bin-path-input'>
                    </div>
                    <a id='submit-update-file' onclick='installCustomPaths()' class='btn save disable'>Update</a>
                </div>
            </div>
        </div>
    </div>
    <div class="container" id="automation-container">
        <div id='local-header-bar' class="header-container">
            <h2>Install from local directory</h2>
        </div>
        <div id='local' class='column'>
            <div class='set'>
                <div class='set-inputs'>
                    <div class='row'>
                        <label for='firmware-file' id='file-update-label'>Choose file</label>
                        <input type='file' name='update' id='firmware-file' onchange='fillUpdateInput(this)' style=display:none>
                        <a id='submit-update-file' onclick='submitUpdate()' class='btn save disable'>Update</a>
                    </div>
                </div>
            </div>
        </div>
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
            <div class='general-info'>
                <div class='repo-name'>
                    ${repo.name}
                </div>
                <div class='repo-description'>
                    ${repo.description}
                </div>
            </div>
            <div class='btn-container'>
                <a onclick='fetchRepoInfo(this)' id='getRepoInfo-${index}' class='btn edit'>Check versions</a>
            </div>
        </div>`;
        return child.firstChild;
    }

    const createRepoMoreInfo = (moreInfo,index) => {
        const options = moreInfo.reduce((prev,info) => {
            return prev + `<option value='${info.version}'>${info.version}${info.current?' (last)':''}</option>`
        },'');
        let child = document.createElement('div');
        child.innerHTML = `<div class='row more-info' id='rowRepoInfo-${index}'>
            <div class='repo-info'>
                <select id='version-${index}' name='versions'>${options}</select>
            </div>
            <div class='btn-container'>
                <a onclick='install(this)' id='getRepoInfo-${index}' class='btn save'>Install</a>
            </div>
        </div>`;
        return child.firstChild;
    }

    // Fetch all project repo available
    const fetchRepoList = async () => {
        try {
            await fetch(window.location.href + 'repo/list/request');
            // Avoid connection_reset from backend
            await delay(2000);
            let listRetrieved = false;
            while(!listRetrieved) {
                let resp = await fetch(window.location.href + 'repo/list');
                let contentType = resp.headers.get("content-type");
                if(contentType && contentType.indexOf("text/json") !== -1) {
                    repos = await resp.json();
                    const container = document.getElementById('remote');
                    repos.forEach((repo,index) => {
                        container.appendChild(createRepoRow(repo,index));
                    });
                    listRetrieved = true;
                } else {
                    await delay(1000);
                }
            }
        } catch (err) {
            console.error(`Error: ${err}`);
        }
    }

    const fetchRepoInfo = async (element) => {
        document.getElementById('page-loader').classList.remove('hidden');
        const repoIndex = element.id.split('-')[1];
        const repo = repos[repoIndex];
        var data = new FormData();
        data.append( "repo", repo.repoRawPath );
        try {
            await fetch(window.location.href + 'repo/info/request', {
                method: 'POST',
                body:data
            });
            // Avoid connection_reset from backend
            await delay(2000);
            let informationRetrieved = false;
            while(!informationRetrieved) {
                let resp = await fetch(window.location.href + 'repo/info');
                let contentType = resp.headers.get("content-type");
                if(contentType && contentType.indexOf("text/json") !== -1) {
                    const repoMoreInfo = await resp.json();
                    const repoRow = document.getElementById(`rowRepo-${repoIndex}`);
                    if (repoRow.childElementCount>2) {
                        repoRow.replaceChild(repoRow.lastElementChild,createRepoMoreInfo(repoMoreInfo,repoIndex));
                    } else {
                        repoRow.appendChild(createRepoMoreInfo(repoMoreInfo,repoIndex));
                    }
                    informationRetrieved = true;
                } else {
                    await delay(1000);
                }
            }
            document.getElementById('page-loader').classList.add('hidden');
        } catch (err) {
            console.error(`Error: ${err}`);
        }
    }

    const install = async (element) => {
        const repoIndex = element.id.split('-')[1];
        const repo = repos[repoIndex];
        const selectedVersion = document.getElementById(`version-${repoIndex}`).value;

        document.getElementById('blocker').classList.remove('hidden');
        document.getElementById('blocker-title').innerText = `Downloading repo ${repo.name} version ${selectedVersion}.\n Reload the page manually in few minutes.`;
        
        var data = new FormData();
        data.append( "binPath", repo.repoRawPath + `${selectedVersion}/espinstall.ino.bin` );
        data.append( "spiffsPath", repo.repoRawPath + `${selectedVersion}/spiffs.bin` );
        try {
            const res = await fetch(window.location.href + 'repo/install/request', {
                method: 'POST',
                body:data
            });
        } catch (err) {
            console.error(`Error: ${err}`);
            document.getElementById('blocker').classList.add('hidden');
        }
    } 

    const installCustomPaths = async (element) => {
        const binPath = document.getElementById('bin-path-input').value;
        const spiffPath = document.getElementById('spiff-path-input').value;

        document.getElementById('blocker').classList.remove('hidden');
        document.getElementById('blocker-title').innerText = `Downloading repo ${binPath}.\n Reload the page manually in few minutes.`;
        
        var data = new FormData();
        data.append( "binPath", binPath);
        data.append( "spiffsPath", spiffPath);
        try {
            const res = await fetch(window.location.href + 'repo/install/request', {
                method: 'POST',
                body:data
            });
        } catch (err) {
            console.error(`Error: ${err}`);
            document.getElementById('blocker').classList.add('hidden');
        }
    }

    // Events
    window.onload = async () => {
        await fetchRepoList();
        document.getElementById('page-loader').classList.add('hidden');
    };
</script>
<!-- HTML_SCRIPT_END -->
)=====";