import React, { useEffect, useState } from 'react';
import './App.css';

const API_BASE = 'http://localhost:5000';

// Map line color names to CSS class
const lineColorToClass: Record<string, string> = {
  Blue: 'Blue',
  Red: 'Red',
  Yellow: 'Yellow',
  Green: 'Green',
  Pink: 'Pink',
  Orange: 'Orange',
  Violet: 'Violet',
  Magenta: 'Magenta',
  Aqua: 'Aqua',
  Grey: 'Grey',
  Rapid: 'Rapid',
};

function parsePathOutput(output: string) {
  // Example output:
  // Shortest Distance: 12.3 km\nPath:\nA -> B [Blue]\nB -> C [Red]\n...
  const lines = output.split('\n');
  let distance = '';
  let pathLines: {from: string, to: string, color: string}[] = [];
  let inPath = false;
  for (let line of lines) {
    if (line.startsWith('Shortest Distance:')) {
      distance = line;
    } else if (line.startsWith('Path:')) {
      inPath = true;
    } else if (inPath && line.includes('->')) {
      // e.g. A -> B [Blue]
      const match = line.match(/(.+) -> (.+) \[(.+)\]/);
      if (match) {
        pathLines.push({from: match[1].trim(), to: match[2].trim(), color: match[3].trim()});
      }
    }
  }
  return { distance, pathLines };
}

function App() {
  const [stations, setStations] = useState<string[]>([]);
  const [src, setSrc] = useState('');
  const [dest, setDest] = useState('');
  const [result, setResult] = useState('');
  const [loading, setLoading] = useState(false);
  const [parsedPath, setParsedPath] = useState<{distance: string, pathLines: {from: string, to: string, color: string}[]} | null>(null);

  useEffect(() => {
    fetch(`${API_BASE}/stations`)
      .then(res => res.json())
      .then(data => {
        setStations(data.stations || []);
      });
  }, []);

  const handleQuery = async () => {
    setResult('');
    setParsedPath(null);
    setLoading(true);
    if (!src || !dest) {
      setResult('Please select both source and destination.');
      setLoading(false);
      return;
    }
    let endpoint = '/distance';
    let body: any = { src, dest };
    try {
      const res = await fetch(`${API_BASE}${endpoint}`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(body),
      });
      const data = await res.json();
      setResult(data.output);
      setParsedPath(parsePathOutput(data.output));
    } catch (e) {
      setResult('Error connecting to backend.');
    }
    setLoading(false);
  };

  // Calculate number of stations in between (nodes in path - 2)
  const numStationsInBetween = parsedPath && parsedPath.pathLines.length > 0
    ? parsedPath.pathLines.length - 1
    : 0;

  return (
    <div className="metro-container">
      <div className="metro-left">
        <div className="metro-header-row">
          <h1>Delhi Metro App</h1>
        </div>
        <div>
          <label>Source Station: </label>
          <select value={src} onChange={e => setSrc(e.target.value)}>
            <option value="">Select source</option>
            {stations.map(st => <option key={st} value={st}>{st}</option>)}
          </select>
        </div>
        <div>
          <label>Destination Station: </label>
          <select value={dest} onChange={e => setDest(e.target.value)}>
            <option value="">Select destination</option>
            {stations.map(st => <option key={st} value={st}>{st}</option>)}
          </select>
        </div>
        <div className="metro-btn-row">
          <button onClick={handleQuery} disabled={loading}>Shortest Distance Path</button>
        </div>
        <div>
          <h3 style={{textAlign: 'center'}}>Result:</h3>
          {loading ? (
            <div className="metro-path-output">Loading...</div>
          ) : parsedPath && parsedPath.pathLines.length > 0 ? (
            <div className="metro-path-output">
              <div className="distance-label">{parsedPath.distance}</div>
              <div style={{fontSize: '1.1em', color: '#0078d7', fontWeight: 700, marginBottom: '0.5em'}}>
                Distance: {parsedPath.distance.replace('Shortest Distance:', '').trim()}
              </div>
              <div className="stations-label">Number of stations in between: {numStationsInBetween}</div>
              <div className="metro-path-steps">
                {parsedPath.pathLines.map((step, idx) => (
                  <React.Fragment key={idx}>
                    <span>{step.from}</span>
                    <span className={`metro-arrow ${lineColorToClass[step.color] || ''}`}>→</span>
                    {idx === parsedPath.pathLines.length - 1 && <span>{step.to}</span>}
                  </React.Fragment>
                ))}
              </div>
            </div>
          ) : (
            <pre>{result}</pre>
          )}
        </div>
      </div>
      <div className="metro-right">
        <h3>All Stations</h3>
        <ul className="metro-station-list" style={{ maxHeight: 600, overflowY: 'auto' }}>
          {stations.map(st => (
            <li key={st}><span className="metro-icon" role="img" aria-label="metro">🚇</span>{st}</li>
          ))}
        </ul>
      </div>
    </div>
  );
}

export default App;
