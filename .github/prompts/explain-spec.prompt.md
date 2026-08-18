---
agent: "agent"
tools: ["search/codebase", "vscode/askQuestions"]
description: "Summarize specification documents"
---

Your goal is to summarize the specification document named ${input:spec_name}. If the name was not supplied, use #tool:vscode/askQuestions to ask the user for it before proceeding. You should focus on the key points, main ideas, and important details that are relevant to the topic. Avoid including unnecessary information or personal opinions. Use your own words to create a summary that is easy to understand and captures the essence of the original documents.

Use the #tool:vscode/askQuestions to ask for the spec_name if not provided. The specifications are located in docs\specs. If no matching file is found in docs\specs, inform the user that the specification could not be located and ask them to verify the name.

<!-- /explain-spec spec_name=db-step-04 if name not provided will be asked -->
<!-- https://code.visualstudio.com/docs/agent-customization/prompt-files#_frequently-asked-questions -->
