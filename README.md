# cn-external

# ns-3 Execution Commands

## 1. Open Ubuntu 24.04

```bash
wsl -d Ubuntu-24.04
```

## 2. Navigate to ns-3

```bash
cd ~/ns-allinone-3.42/ns-3.42
```

## 3. Run Simulations

### Q7 – Star Topology

```bash
./ns3 run scratch/cn-lab/star-topology.cc
```

### Q8 – Stop-and-Wait ARQ

```bash
./ns3 run scratch/cn-lab/stopwait.cc
```

### Q9 – Go-Back-N ARQ

```bash
./ns3 run scratch/cn-lab/gobackn.cc
```

## 4. Check Generated XML Files

```bash
ls -lh *.xml
```

## 5. Open NetAnim

Open a second Ubuntu terminal:

```bash
wsl -d Ubuntu-24.04
```

```bash
cd ~/ns-allinone-3.42/netanim-3.109
```

```bash
QT_XCB_GL_INTEGRATION=none QT_OPENGL=software ./NetAnim
```

## 6. XML Files

| Experiment | XML File |
|---|---|
| Q7 – Star Topology | `star-topology.xml` |
| Q8 – Stop-and-Wait | `stopwait.xml` |
| Q9 – Go-Back-N | `gobackn.xml` |

