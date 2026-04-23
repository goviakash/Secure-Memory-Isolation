const express = require("express");
const { exec } = require("child_process");
const fs = require("fs");
const path = require("path");

const app = express();
const PORT = 3000;

// 🔐 Run C program and return output
app.get("/run", (req, res) => {
    console.log("Running C program...");

    const execCommand = process.platform === "win32" ? "run.exe" : "./run";

    exec(execCommand, (error) => {
        if (error) {
            console.error("Execution error:", error);
            return res.send("Error running program");
        }

        const filePath = path.join(__dirname, "result.txt");

        fs.readFile(filePath, "utf8", (err, data) => {
            if (err) {
                console.error("File read error:", err);
                return res.send("No output found");
            }

            res.send(data);
        });
    });
});

// Serve static files (index.html, result.txt, etc.)
app.use(express.static(__dirname));

// Start server
app.listen(PORT, () => {
    console.log(`🚀 Server running at http://localhost:${PORT}`);
});