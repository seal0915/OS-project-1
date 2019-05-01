<h2>[2019OS] Project1: Process Scheduling</h2>

<h3>設計</h3>

<p>主架構參考了一篇Github上的repo，透過研究這個已經完備的repo架構，畫出流程圖，並精簡、改良了裡面imply不清楚的地方。
核心設計在於用*set_scheduler()*函數實作*set_low_priority()*和*set_high_priority()*兩個函數。
透過這兩個函數可以將特定某個process設為執行而其他設為停止，以此達到自由實作各種schedule policy的模式。</p>

詳細流程如下：

	1. 處理Input，製作processList，並依arrivalTime排序。
	2. run scheduler，開始 schedule processing 和計時。

scheduler主迴圈的詳細內容如下：

	process_scheduling(){
		1.迴圈 直到所有process跑過為止
			1. 回收已經結束的process。
			2. 開啟所有已經arrived的process，並預先令其停止(set low)。 (startprocess()) 
			3. 決定唯一一個process (實作schedule policy去決定)，使CPU執行之(set high)。
				如果已有process在執行中，用set low停止他(context switch)。
			4. 跑一秒，並減少正在跑的process其requiredTime一秒
	}
為了避免被其他程序干擾，所有的process都會被指定到另一個cpu，如果可行的話，甚至可以用*isolcpus*來騰出一個cpu。
在process被開啟後，會從scheduler中fork()出一個process，並在新的process中執行run_process()這個函數。
只有被set high的process會跟著scheduler的時鐘跑。

run_process()內容：

	run_process(){
		1.迴圈 直到該process之requiredTime = 0為止
			1. 跑一秒。
		2.用gettimesofday()來取得process開始和結束Time
		3.用system函數，使用shell中的echo，把測試輸出直接塞到/dev/kmsg裡，使其可以被dmesg找到。
	}
		
在run_process()結束後，scheduler所紀錄的requiredTime也歸零了（理想狀態），回收process。

---
<h3>範例測試結果</h3>

範例輸入：

	FIFO
	3
	P1 1 10
	P2 2 5
	P3 2 7

範例輸出：

	P1 5011
	P2 5012
	P3 5013

---
<h3>比較</h3>

在CPU非常busy時，如果沒有isolcpus出一個cpu來測試，會導致scheduler和process上的計時器不一致，從而導致實驗結果和預期有所誤差。
由於沒能夠製造足夠強的CPU busy的狀態，因此還沒有觀察到這個於其中的誤差，也不存在比較。 # 待改

---
<h3>組員貢獻</h3>

架構圖、架構設計：李豈翔、林震  
coding：李豈翔  
scheduling emulation function: 林震  
next_process function: 林震  
report.md：李豈翔  
schedule_simulation.c 賴億泓  
github repository: 林秉駿  


<h4>Reference:</h4>

	https://github.com/andy920262/OS2016  參考的code
