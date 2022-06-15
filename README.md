# IndControl
1.取消pdb
项目-〉属性-〉配置属性-〉链接器-〉调试-〉生成调试信息，这里设为“否”。
2.取消iobj ipdb
项目-〉属性-〉配置属性-〉链接器-〉优化-〉链接代码时间生成 ，这里设为“默认设置”。
3.取消exp 

step1: 项目->属性->C/C+±>General->Debug Information Format->Program Database for Edit & Continue (/ZI)
step2: 项目->属性->C/C+±>Optimization->Optimization->Disabled(/Od)
step3: 项目->属性->Linker->Debugging->Generate Debug Info->Yes(/DEBUG)
